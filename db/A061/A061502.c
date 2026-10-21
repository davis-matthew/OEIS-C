/*
 * A061502  a(n) = Sum_{k<=n} tau(k)^2, where tau = number of divisors
 *          function A000005.
 * https://oeis.org/A061502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} tau(k)^2 tau(k) = number of divisors of k
 *            (OEIS A000005) The implementation stores a divisor‑count sieve
 *            and a running sum of the squares of the counts. It keeps the
 *            largest computed n in static storage so that arbitrary order of
 *            calls is supported
 * Range:     u8 n <= 19, u16 n <= 901, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static idx_t max_n = 0;
    static seq_t *tau  = NULL;   /* tau[i] = number of divisors of i   */
    static seq_t *pref = NULL;   /* pref[i] = sum_{k<=i} tau(k)^2      */

    /* n=0 is outside the OEIS range – return 0 */
    if (n == 0) return 0;

    /* rebuild the arrays if a larger n is requested */
    if (n > max_n) {
        idx_t new_n = n;
        /* Allocate new arrays using malloc – seq_t is either an
           unsigned integer type or a typedef of a POD integer type,
           so raw allocation is safe.  No constructor is needed. */
        seq_t *new_tau  = (seq_t *)malloc((new_n + 1) * sizeof(seq_t));
        seq_t *new_pref = (seq_t *)malloc((new_n + 1) * sizeof(seq_t));
        if (!new_tau || !new_pref) {
            /* allocation failed – abort */
            exit(EXIT_FAILURE);
        }

        /* Initialise divisor counts to 0 (leave index 0 unused) */
        for (idx_t i = 1; i <= new_n; ++i) new_tau[i] = 0;

        /* Divisor–count sieve: for each i add 1 to all its multiples */
        for (idx_t i = 1; i <= new_n; ++i) {
            for (idx_t j = i; j <= new_n; j += i) {
                new_tau[j] = new_tau[j] + 1;
            }
        }

        /* Compute the prefix sums of the squares of tau */
        new_pref[0] = 0;
        for (idx_t i = 1; i <= new_n; ++i) {
            seq_t sq = new_tau[i] * new_tau[i];
            new_pref[i] = new_pref[i - 1] + sq;
        }

        /* Replace the old arrays (leaving the old memory for the
           program lifetime – acceptable for the expected use case). */
        tau  = new_tau;
        pref = new_pref;
        max_n = new_n;
    }

    /* Return the requested value */
    return pref[n];
}
