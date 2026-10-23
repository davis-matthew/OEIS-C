/*
 * A036207  Log base 7 (n) mod 239.
 * https://oeis.org/A036207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = log₇(n) (mod 239) (= the exponent x with 7ˣ ≡ n (mod
 *            239), if n ≡ 0 (mod 239) we return 0)
 * Range:     u8 n <= 238, u16 n <= 238, u32 n <= 238, u64 n <= 238,
 *            u128 n <= 238, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t log_tbl[239];
    static bool init = false;

    if (!init) {
        init = true;
        const seq_t MOD  = 239;
        const seq_t BASE = 7;
        seq_t power = 1;

        /* 7⁰,7¹,…,7²³⁷ all distinct modulo 239 because 239 is prime.
           The exponent 238 gives 7²³⁷ ≡ 1, so we stop before overwriting
           the 0‑exponent value. */
        for (idx_t exp = 0; exp < 238; ++exp) {
            log_tbl[(size_t)power] = (seq_t)exp;
            power = (power * BASE) % MOD;
        }
    }

    idx_t r = n % 239;
    if (r == 0)          /* log undefined for 0 (mod 239) – return 0 */
        return 0;
    return log_tbl[(size_t)r];
}
