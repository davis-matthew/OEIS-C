/*
 * A050666  Primes p such that number of primes produced according to rules
 *          stipulated in Honaker's A048853 is 4.
 * https://oeis.org/A050666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   compute A050666 – primes p such that exactly four primes are
 *            produced by changing any single decimal digit of p to a
 *            different digit (without leading zeros)
 * Range:     u8 n <= 1, u16 n <= 33, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static const idx_t PRIME_LIMIT = 20000000;   /* enough for large n */

/* Count primes obtained by replacing one digit of p */
static int count_prime_replacements(seq_t p) {
    /* extract digits in most‑significant first order */
    seq_t digits[30];     /* 128 bits -> <30 decimal digits */
    int len = 0;
    seq_t temp = p;
    if (temp == 0) { /* not a prime in this context */
        digits[0] = 0;
        len = 1;
    } else {
        while (temp > 0) {
            digits[len++] = temp % 10;
            temp /= 10;
        }
        /* reverse to most‑significant first */
        for (int i = 0; i < len / 2; ++i) {
            seq_t t = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = t;
        }
    }

    int count = 0;
    for (int pos = 0; pos < len && count <= 4; ++pos) {
        seq_t orig = digits[pos];
        for (int d = 0; d <= 9 && count <= 4; ++d) {
            if (d == orig) continue;            /* same digit */
            if (pos == 0 && d == 0) continue;    /* leading zero forbidden */

            /* construct new number */
            seq_t new_num = 0;
            for (int i = 0; i < len; ++i) {
                seq_t dig = (i == pos) ? (seq_t)d : digits[i];
                new_num = new_num * 10 + dig;
            }

            if (is_prime((idx_t)new_num))
                ++count;
        }
    }
    return count;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;     /* offset is 1 – caller should not ask for 0 */

    /* ensure primes list is built once */
    if (!prime_list) {
        size_t cnt;
        build_primes_upto(PRIME_LIMIT, &prime_list, &cnt);
        prime_count = cnt;
    }

    idx_t found = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        seq_t p_val = (seq_t)prime_list[i];
        if (count_prime_replacements(p_val) == 4) {
            ++found;
            if (found == n)
                return p_val;
        }
    }

    /* If we reach here, limit was insufficient; return 0 (should not happen). */
    return 0;
}
