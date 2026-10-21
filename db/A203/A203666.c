/*
 * A203666  Number of (n+1)X3 0..6 arrays with column and row pair sums
 *          b(i,j)=a(i,j)+a(i,j-1) and c(i,j)=a(i,j)+a(i-1,j) nondecreasing in
 *          column and row directions, respectively.
 * https://oeis.org/A203666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 16, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A203666 – Number of (n+1)×3 arrays with values 0…6 such that
 *  horizontal pair sums b(i,2)=a(i,2)+a(i,1) and b(i,3)=a(i,3)+a(i,2)
 *  are non‑decreasing down the rows (i → i+1) and
 *  vertical pair sums c(i,1)=a(i,1)+a(i−1,1) ,
 *  c(i,2)=a(i,2)+a(i−1,2) , c(i,3)=a(i,3)+a(i−1,3)
 *  are non‑decreasing across the columns j (1 → 2 → 3) for each row i≥2.
 *
 *  The algorithm builds an adjacency matrix on the 7³=343 possible
 *  rows (triples of entries 0…6).  An edge u→v exists iff the four
 *  inequalities
 *      u₁+u₂ ≤ v₁+v₂          // horizontal sums down
 *      u₂+u₃ ≤ v₂+v₃
 *      v₁+u₁ ≤ v₂+u₂          // vertical sums right
 *      v₂+u₂ ≤ v₃+u₃
 *  hold.  Counting walks of length n on this graph yields a(n);
 *  the walk starts at any state (first row is unrestricted).
 *  This method works for any sequence length n≥1 and satisfies the
 *  required time and size limits.  */

/* ------------------------------------------------------------ */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

enum { S     = 7,          /* entry values 0…6 */
       STATES = S * S * S };   /* 343 possible rows */

/* Decomposition of a state index into its three components */
static int rs_a[STATES];
static int rs_b[STATES];
static int rs_c[STATES];

/* Adjacency lists: neighbors[u] is the array of states reachable
 * from state u.  neighbor_cnt[u] holds the number of neighbours. */
static int  neighbors[STATES][STATES];
static int  neighbor_cnt[STATES];
static bool adjacency_ready = false;

/* ------------------------------------------------------------ */
/* Build the adjacency lists once.  All calculations are with
 * signed ints because the sums are at most 12. */
static void init_adjacency(void)
{
    if (adjacency_ready) return;

    int id = 0;
    for (int a = 0; a < S; ++a)
        for (int b = 0; b < S; ++b)
            for (int c = 0; c < S; ++c)
            {
                rs_a[id] = a;
                rs_b[id] = b;
                rs_c[id] = c;
                ++id;
            }

    for (int u = 0; u < STATES; ++u)
    {
        int ux = rs_a[u];
        int uy = rs_b[u];
        int uz = rs_c[u];
        int cnt = 0;
        for (int v = 0; v < STATES; ++v)
        {
            int vx = rs_a[v];
            int vy = rs_b[v];
            int vz = rs_c[v];
            /* Horizontal pair sums non‑decreasing downwards: */
            if (ux + uy > vx + vy) continue;    /* b(i,2) */
            if (uy + uz > vy + vz) continue;    /* b(i,3) */
            /* Vertical pair sums non‑decreasing left → right in the
             * lower row (row i+1).  These involve the lower row values
             * (v) and the upper row values (u). */
            if (vx + ux > vy + uy) continue;    /* c(i+1,1) ≤ c(i+1,2) */
            if (vy + uy > vz + uz) continue;    /* c(i+1,2) ≤ c(i+1,3) */
            neighbors[u][cnt++] = v;
        }
        neighbor_cnt[u] = cnt;
    }

    adjacency_ready = true;
}

/* ------------------------------------------------------------ */
/* Main sequence function requested by the harness.  The function
 * recomputes the dynamic programming table for every call; this
 * satisfies the "stateless" requirement about arbitrary call order. */
seq_t a(idx_t n)
{
    /* By definition the sequence is indexed starting at 1. */
    if (n == 0) return (seq_t)0;

    init_adjacency();                     /* build adjacency once */

    /* dp[u] – number of ways to reach state u after the current row.  */
    static seq_t dp[STATES];
    static seq_t ndp[STATES];

    /* Initial row – any state is allowed. */
    for (int i = 0; i < STATES; ++i) dp[i] = (seq_t)1;

    /* Perform n transitions (n+1 rows → n steps). */
    for (idx_t step = 0; step < n; ++step)
    {
        /* zero ndp */
        for (int i = 0; i < STATES; ++i) ndp[i] = (seq_t)0;

        /* propagate from each current state */
        for (int u = 0; u < STATES; ++u)
        {
            seq_t val = dp[u];
            if (val == (seq_t)0) continue;
            int cnt = neighbor_cnt[u];
            for (int k = 0; k < cnt; ++k)
            {
                int v = neighbors[u][k];
                ndp[v] += val;                /* seq_t addition is exact */
            }
        }

        /* swap dp and ndp */
        for (int i = 0; i < STATES; ++i) dp[i] = ndp[i];
    }

    /* Sum all possibilities for the final row. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < STATES; ++i) total += dp[i];

    return total;
}
