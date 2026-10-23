/*
 * A104946  Primes from merging of 3 successive digits in decimal expansion of
 *          the Champernowne Constant.
 * https://oeis.org/A104946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   a(n) = nth prime found by scanning consecutive 3‑digit windows
 *            of Champernowne's constant (base 10), ignoring windows whose
 *            first digit is 0
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, offset starts at 1 */

    idx_t   found = 0;
    int     last2 = -1, last1 = -1;    /* last two digits seen */

    for (uint64_t num = 1; ; ++num) {
        /* Extract digits of num into array (most significant first) */
        uint64_t m = num;
        int digits[20];
        int dlen = 0;
        do {
            digits[dlen++] = m % 10;
            m /= 10;
        } while (m);

        for (int i = dlen - 1; i >= 0; --i) {
            int d = digits[i];

            if (last2 >= 0 && last2 != 0) {
                int val = last2 * 100 + last1 * 10 + d;
                /* test primality of val (3‑digit values only) */
                bool is_prime = true;
                if (val <= 1) {
                    is_prime = false;
                } else if (val % 2 == 0) {
                    is_prime = (val == 2);
                } else {
                    for (int p = 3; p * p <= val; p += 2) {
                        if (val % p == 0) {
                            is_prime = false;
                            break;
                        }
                    }
                }
                if (is_prime) {
                    ++found;
                    if (found == n) {
                        return (seq_t)val;
                    }
                }
            }

            last2 = last1;
            last1 = d;
        }
    }
}
