/*
 * A139118  Numbers with a nonprime number of divisors.
 * https://oeis.org/A139118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   numbers whose divisor function τ(n) is NOT prime (OEIS A139118).
 *            The sequence is generated on demand by keeping a list of the
 *            terms found so far and extending it until the requested index is
 *            reached. The subsequence is constructed efficiently using a
 *            table of smallest prime factors built with the...
 * Range:     u8 n <= 192, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, is_prime_ul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_spf.h"
#include "../utils/seq_realloc.h"
#include "../utils/is_prime_ul.h"

seq_t a(idx_t n)
{
    /* Static storage for the generated sequence */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;   /* capacity of seq */
    static size_t seq_len = 0;   /* number of values stored */

    /* Smallest prime factor table and its current limit */
    static idx_t *spf = NULL;
    static idx_t spf_limit = 0;

    /* Last integer processed when extending the sequence */
    static idx_t last_num = 0;

    /* Extend the sequence until it contains at least n terms */
    while (seq_len < n) {
        /* Make sure spf covers the next integer to test */
        if (last_num + 1 > spf_limit) {
            idx_t new_limit = spf_limit ? spf_limit << 1 : 2;
            while (new_limit <= last_num + 1) new_limit <<= 1;

            free(spf);                     /* free old table */
            build_spf(new_limit, &spf);    /* new table */
            spf_limit = new_limit;
        }

        idx_t num = last_num + 1;
        unsigned long long tau = 1;       /* divisor count */

        if (num == 1) {
            tau = 1;
        } else {
            idx_t t = num;
            while (t > 1) {
                idx_t p = spf[t];
                idx_t e = 0;
                while (t % p == 0) {
                    t /= p;
                    ++e;
                }
                tau *= (unsigned long long)(e + 1);
            }
        }

        /* Record the number if τ(num) is non‑prime */
        if (!is_prime_ul(tau)) {
            if (seq_len == seq_cap) {                /* grow array */
                size_t new_cap = seq_cap ? seq_cap << 1 : 256;
                seq = seq_realloc(seq, seq_cap, new_cap);
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)num;
        }

        ++last_num;
    }

    /* Return the requested term (1‑based index) */
    return seq[n - 1];
}
