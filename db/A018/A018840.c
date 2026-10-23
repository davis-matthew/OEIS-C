/*
 * A018840  Number of steps for {2,3} fairy knight to reach (n,0) on infinite
 *          chessboard.
 * https://oeis.org/A018840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 763, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A018840
   a(n) = a(n-6) + 2   for n ≥ 18.
   Initial values:
   a(0)=0, a(1)=5, a(2)=4, a(3)=5, a(4)=2, a(5)=5, a(6)=2, a(7)=5,
   a(8)=4, a(9)=5, a(10)=4, a(11)=7, a(12)=4, a(13)=5, a(14)=6,
   a(15)=7, a(16)=6, a(17)=7 */

#include <stdlib.h>
#include <stdbool.h>

/* Precomputed distances for 0 ≤ n ≤ 17.  Computed by BFS on a
   sufficiently large bounding box. */
static int base_int[18];
static bool base_computed = false;

/* ------------------------------------------------------------ */
/* Compute distances a(n) for 0 ≤ n ≤ 17 by BFS. */
static void compute_base(void)
{
    /* Bounding box: coordinates in [-B, B].  B is chosen large
       enough to contain shortest paths to (17,0). */
    const int B = 50;
    const int R = 2*B + 1;            /* size of one side of grid */
    const size_t SZ = (size_t)R * R;  /* total number of cells */

    int *dist  = (int*)malloc(SZ * sizeof(int));
    int *queue = (int*)malloc(SZ * sizeof(int));
    if (!dist || !queue) { /* allocation failed */
        exit(1);
    }

    for (size_t i = 0; i < SZ; ++i)
        dist[i] = -1;

    /* starting position (0,0) */
    const int start_idx = (B) * R + (B);
    dist[start_idx] = 0;
    int head = 0, tail = 1;
    queue[0] = start_idx;

    /* Knight moves */
    const int moves[8][2] = {
        { 2,  3}, { 2, -3}, {-2,  3}, {-2, -3},
        { 3,  2}, { 3, -2}, {-3,  2}, {-3, -2}
    };

    /* BFS */
    while (head < tail) {
        int cur = queue[head++];
        int cx  = (cur % R) - B;
        int cy  = (cur / R) - B;
        int dcur = dist[cur];

        for (int m = 0; m < 8; ++m) {
            int nx = cx + moves[m][0];
            int ny = cy + moves[m][1];
            if (nx < -B || nx > B || ny < -B || ny > B)
                continue;
            int nidx = (ny + B) * R + (nx + B);
            if (dist[nidx] == -1) {
                dist[nidx] = dcur + 1;
                queue[tail++] = nidx;
            }
        }
    }

    /* Record distances to (n,0) for n = 0 … 17 */
    for (idx_t x = 0; x <= 17; ++x) {
        int idx = (0 + B) * R + (x + B);
        base_int[(size_t)x] = dist[idx];
    }

    free(dist);
    free(queue);
    base_computed = true;
}

/* ------------------------------------------------------------ */
/* Return a(n) for any n ≥ 0.  Recurrence: a(n)=a(n-6)+2 for n≥18. */
seq_t a(idx_t n)
{
    if (!base_computed)
        compute_base();

    if (n <= 17) {
        return (seq_t)base_int[(size_t)n];
    }

    /* Reduce n by repeatedly subtracting 6 until it lies in [12,17] */
    idx_t rem   = n % 6;
    idx_t m     = rem + 12;                 /* 12 ≤ m ≤ 17 */
    idx_t k     = (n - m) / 6;              /* k ≥ 0 */
    seq_t val   = (seq_t)base_int[(size_t)m];
    seq_t inc   = (seq_t)2 * (seq_t)k;      /* add 2*k */
    val += inc;
    return val;
}
