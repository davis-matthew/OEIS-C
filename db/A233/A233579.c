/*
 * A233579  Numbers n such that the denominator/6 of Bernoulli(n) is congruent
 *          to {11, 17, 23, 25 or 29} modulo 30.
 * https://oeis.org/A233579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A233579: numbers n such that (denominator of B_n)/6 ≡ {11,17,23,25,29} (mod 30).
   The function generates the sequence by brute‑force testing of even n using Von
   Staudt–Clausen's description of Bernoulli denominators.  Each candidate n
   is examined by iterating over its divisors d; whenever d+1 is prime, that
   prime contributes to the denominator.  The product of all such primes is
   maintained modulo 180 (since 6⋅30=180).  If the resulting residue modulo 30
   after division by 6 belongs to the target set, n is stored in the sequence.*/

#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Static storage for the sequence values */
    static seq_t *seq      = NULL;
    static size_t seq_len  = 0;
    static size_t seq_cap  = 0;
    static idx_t  max_checked = 0;   /* last number examined */

    if (seq_len >= n)
        return seq[n - 1];   /* already computed */

    /* start searching from the next even number after the last checked one */
    idx_t start = (max_checked == 0) ? 2 : max_checked + 1;
    if (start % 2 != 0) ++start;          /* ensure even */

    while (seq_len < n) {
        idx_t m   = start;
        int   prod = 1;    /* D(m) modulo 180 */

        /* iterate over all divisors d of m */
        for (idx_t d = 1; d <= m / d; ++d) {
            if (m % d == 0) {
                /* divisor d */
                if (is_prime((idx_t)(d + 1))) {
                    prod = (int)((prod * ((d + 1) % 180)) % 180);
                }
                /* paired divisor m/d */
                idx_t other = m / d;
                if (other != d && is_prime((idx_t)(other + 1))) {
                    prod = (int)((prod * ((other + 1) % 180)) % 180);
                }
            }
        }

        int val6 = prod / 6;           /* D(m)/6, integer for even m */
        int rem  = val6 % 30;          /* residue modulo 30 */

        if (rem == 11 || rem == 17 || rem == 23 || rem == 25 || rem == 29) {
            grow_seq(&seq, &seq_cap, seq_len + 1);
            seq[seq_len++] = (seq_t)m;
        }

        max_checked = m;
        start += 2;                   /* next even number */
    }

    return seq[n - 1];
}
