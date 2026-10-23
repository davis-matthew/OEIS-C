/*
 * A180879  Number of permutations p() of 1..n+2 with centered difference
 *          p(i+1)-p(i-1) < 0 exactly once.
 * https://oeis.org/A180879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(n+2,ceil((n+2)/2))*E[ceil((n+2)/2)] +
 *            binom(n+2,floor((n+2)/2))*E[floor((n+2)/2)] where E[k] is the
 *            Eulerian number A(k,1) (permutations of k with exactly one
 *            descent)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/*--------------------------------------------------------------------
   Utility: binomial coefficient  C(n,k)  (exact divide in each step)
--------------------------------------------------------------------*/
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = (r * ((seq_t)n - k + i)) / (seq_t)i;
    }
    return r;
}

/*--------------------------------------------------------------------
   Pre‑computed array of E(k):  number of permutations of k with
   exactly one descent (Eulerian number A(k,1)).
   The array is filled on the first call to a().
--------------------------------------------------------------------*/
#define MAX_K 5001          /* sufficient for n ≤ 10000 */
static seq_t E[MAX_K + 1];
static bool E_ready = false;

static void ensure_E(void)
{
    if (E_ready) return;
    E[0] = 0;          /* unused, but harmless */
    E[1] = 0;
    E[2] = 1;
    for (idx_t i = 3; i <= MAX_K; ++i) {
        E[i] = (seq_t)2 * E[i-1] + (seq_t)(i-1);
    }
    E_ready = true;
}

/*--------------------------------------------------------------------
   Main function
--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1 */

    idx_t m = n + 2;
    idx_t a_sz = (m + 1) / 2;     /* ceil(m/2) */
    idx_t b_sz = m / 2;           /* floor(m/2) */

    ensure_E();

    seq_t term1 = 0, term2 = 0;
    if (a_sz >= 2) term1 = binom(m, a_sz) * E[a_sz];
    if (b_sz >= 2) term2 = binom(m, b_sz) * E[b_sz];

    return term1 + term2;
}
