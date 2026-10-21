/*
 * A064898  Stirling transform of derangements numbers.
 * https://oeis.org/A064898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = Sum_{k=0..n} Stirling2(n,k) * A000166(k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the Stirling transform of derangements numbers. */

#include <stddef.h>   /* For size_t, if needed */

/* Maximum n we pre‑allocate storage for. The sequence is fast enough up
   to about 10000, so this covers the required domain. */
#define MAX_N 10000

/* Static tables holding the sequence, the derangements D(k)
   and the current Stirling row S(k). All indices are 0‑based. */
static seq_t A[MAX_N + 1];
static seq_t D[MAX_N + 2];    /* D[0..MAX_N+1] */
static seq_t S[MAX_N + 2];    /* S[0..MAX_N+1] */

/* Highest n that has already been computed. */
static idx_t last_n = 0;
static int initialized = 0;

/* Initialise the base cases once. */
static void init_once(void)
{
    if (initialized)
        return;
    initialized = 1;

    /* Derangement numbers */
    D[0] = (seq_t)1;   /* D(0) = 1 */
    D[1] = (seq_t)0;   /* D(1) = 0 */

    /* Stirling numbers: S(0,0) = 1 */
    S[0] = (seq_t)1;

    /* Sequence value at n = 0 */
    A[0] = D[0] * S[0];
    last_n = 0;
}

/* Compute a(n), for any n (0 <= n <= MAX_N). */
seq_t a(idx_t n)
{
    if (n > MAX_N) {
        /* Unsupported; silently return 0.  The caller may request larger
           values but the problem statement only requires up to ~10000. */
        return (seq_t)0;
    }

    init_once();

    /* If we have already computed up to this n, just return it. */
    if (last_n >= n)
        return A[n];

    /* Compute all intermediate values up to n. */
    for (idx_t m = last_n + 1; m <= n; ++m) {

        /* ---- Derangements D(m) ---- */
        if (m >= 2) {
            D[m] = (seq_t)(m - 1) * (D[m - 1] + D[m - 2]);
        }
        /* D[1] remains 0 from initialization; D[0] stays 1. */

        /* ---- Stirling row S(k) for current n = m ---- */
        for (idx_t k = m; k > 0; --k) {
            S[k] = (seq_t)k * S[k] + S[k - 1];
        }
        S[0] = (seq_t)0;   /* S(m,0) = 0 for m > 0 */

        /* ---- Compute a(m) as the Stirling transform of D ---- */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) {
            sum += S[k] * D[k];
        }
        A[m] = sum;
    }

    last_n = n;
    return A[n];
}
