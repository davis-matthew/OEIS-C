/*
 * A220504  Triangle read by rows: T(n,k) is the total number of appearances
 *          of k as the smallest part in all partitions of n.
 * https://oeis.org/A220504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = T(n‑k,k) + P_k(n‑k) where P_k(m) is the number of
 *            partitions of m with all parts ≥ k
 * Range:     u8 n <= 78, u16 n <= 595, u32 n <= 5886, u64 n <= 7803,
 *            u128 n <= 7803, big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the triangle read by rows: a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), …   */

#include <stdbool.h>
#include "../utils/seq_alloc.h"          /* seq_alloc – allocates seq_t arrays */

/* maximum number of rows pre‑computed.
   For n ≤ 10000 the corresponding row is ≤ 142, so 200 is more than enough. */
#define MAX_ROW 200                     /* rows are numbered 1 .. MAX_ROW */
#define COLS  (MAX_ROW + 1)             /* indices 0 .. MAX_ROW */

/* static storage */
static seq_t *q = NULL;                 /* partitions with parts ≥ k  : q[k][n] */
static seq_t *t = NULL;                 /* T(k,n)  : number of k’s in partitions of n having smallest part k  */
static seq_t *flat = NULL;              /* flattened triangle (a(1),a(2),…)   */
static size_t flat_len = 0;              /* number of entries stored in flat */
static bool initialized = false;

/* index helpers for 1‑D allocation */
#define Q(k,n)  ((k)*(COLS)+(n))
#define T(k,n)  ((k)*(COLS)+(n))

/* initialise data structures and pre‑compute the triangle */
static void init(void)
{
    size_t rows = MAX_ROW + 2;    /* need k up to MAX_ROW+1 for recurrence */
    size_t size = rows * COLS;    /* number of elements in q and t */
    size_t flat_cap = (MAX_ROW + 1) * (MAX_ROW + 2) / 2;   /* enough for all rows */

    /* allocate memory – seq_alloc ensures proper construction for C++ */
    q    = seq_alloc(size);
    t    = seq_alloc(size);
    flat = seq_alloc(flat_cap);

    /* initialise to zero */
    seq_t zero = 0;
    for (size_t i = 0; i < size; ++i) {
        q[i] = zero;
        t[i] = zero;
    }
    for (size_t i = 0; i < flat_cap; ++i)
        flat[i] = zero;

    /* base for k = MAX_ROW+1 : partitions of 0 only */
    q[(MAX_ROW + 1) * COLS + 0] = 1;   /* p_{MAX_ROW+1}(0) = 1 */

    /* compute q[k][n] for k=MAX_ROW down to 1 */
    for (int k = MAX_ROW; k >= 1; --k) {
        for (int n = 0; n <= MAX_ROW; ++n) {
            seq_t a = q[(k + 1) * COLS + n];
            seq_t b = 0;
            if (n >= k)
                b = q[k * COLS + (n - k)];
            q[k * COLS + n] = a + b;
        }
    }

    /* compute t[k][n] for k=MAX_ROW down to 1 */
    for (int k = MAX_ROW; k >= 1; --k) {
        for (int n = 0; n <= MAX_ROW; ++n) {
            seq_t val = 0;
            if (n >= k) {
                val = q[k * COLS + (n - k)] + t[k * COLS + (n - k)];
            }
            t[k * COLS + n] = val;
        }
    }

    /* flatten the triangle into a sequence */
    size_t idx = 0;
    for (int r = 1; r <= MAX_ROW; ++r) {
        for (int k = 1; k <= r; ++k) {
            flat[idx++] = t[k * COLS + r];
        }
    }
    flat_len = idx;
    initialized = true;
}

/* the required function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, but safe */

    if (!initialized)
        init();

    if ((size_t)n > flat_len)        /* n beyond pre‑computed range */
        return (seq_t)0;

    return flat[(size_t)n - 1];
}
