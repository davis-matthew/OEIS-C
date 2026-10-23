/*
 * A036176  Log base 5 (n) mod 73.
 * https://oeis.org/A036176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete_log_5(n) (mod 73) where a(n) is the exponent e
 *            such that 5^e ≡ n (mod 73), with a(1) = 0 and a(n) = 0 for n ≡ 0
 *            (mod 73)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t log5_mod73[73] = {0};
static bool log5_mod73_inited = false;

static void init_log5_mod73(void)
{
    if (log5_mod73_inited)
        return;

    /* 5^0 ≡ 1 (mod 73) → exponent 0 for residue 1 */
    log5_mod73[1] = 0;

    seq_t p = 1;
    for (idx_t exp = 1; exp < 72; ++exp) {   /* never reach exp = 72 */
        p = (p * (seq_t)5) % (seq_t)73;
        log5_mod73[(size_t)p] = exp;
    }

    log5_mod73_inited = true;
}

seq_t a(idx_t n)
{
    init_log5_mod73();
    idx_t r = n % 73;
    if (r == 0)                     /* n divisible by 73 → no discrete log */
        return (seq_t)0;
    return log5_mod73[(size_t)r];
}
