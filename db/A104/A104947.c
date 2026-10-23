/*
 * A104947  Primes from merging of 4 successive digits in decimal expansion of
 *          the Champernowne Constant.
 * https://oeis.org/A104947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search:  primes appearing in any 4‑digit window of the decimal expansion
   of Champernowne's constant 0.123456789101112…  The window is counted
   only if its most significant digit is non‑zero (i.e. the number is
   truly 4‑digit).  The function scans the Champernowne digits until the
   n‑th such prime is found.  It memoises a small sieve of primes up to
   9999 to test primality in O(1). */

#include <stdbool.h>
#include <stdint.h>

/* sieve table for primes ≤ 9999 */
static unsigned char prime_tbl[10000];
static bool prime_tbl_inited = false;

static void init_prime_tbl(void)
{
    for (int i = 0; i < 10000; ++i) prime_tbl[i] = 1;
    prime_tbl[0] = prime_tbl[1] = 0;
    for (int i = 2; i * i < 10000; ++i)
        if (prime_tbl[i])
            for (int j = i * i; j < 10000; j += i)
                prime_tbl[j] = 0;
    prime_tbl_inited = true;
}

seq_t a(idx_t n)
{
    if (!prime_tbl_inited) init_prime_tbl();

    uint64_t found = 0;          /* number of primes seen */
    int val = 0;                 /* current 4‑digit window as integer */
    int len = 0;                 /* number of digits currently in the window */

    for (uint64_t num = 1; ; ++num) {
        /* digits of num in reverse order */
        char buf[20];
        int digits = 0;
        uint64_t tmp = num;
        do {
            buf[digits++] = (char)('0' + (tmp % 10));
            tmp /= 10;
        } while (tmp);

        /* process from most significant to least */
        for (int i = digits - 1; i >= 0; --i) {
            int d = buf[i] - '0';

            if (len < 4) {
                val = val * 10 + d;
                ++len;
                if (len == 4 && val >= 1000 && prime_tbl[val]) {
                    ++found;
                    if (found == n) return (seq_t)val;
                }
            } else { /* len == 4 */
                val = (val % 1000) * 10 + d;   /* drop most significant digit */
                if (val >= 1000 && prime_tbl[val]) {
                    ++found;
                    if (found == n) return (seq_t)val;
                }
            }
        }
    }
}
