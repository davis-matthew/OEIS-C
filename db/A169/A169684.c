/*
 * A169684  Decimal expansion of 2^107 - 1.
 * https://oeis.org/A169684
 *
 * Offset:    33    (a(33) is the first term)
 * Keywords:  nonn, cons, easy, fini, full
 * Formula:   a(n) is the decimal digit at position n-33 of (2^107)-1, the
 *            107‑bit Mersenne prime. For 33 ≤ n ≤ 62 the value is defined as
 *            the corresponding digit; out‑of‑range calls return 0
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Decimal expansion of 2^107 - 1 */
    static const char digits[] =
        "162259276829213363391578010288127";
    const size_t len = sizeof(digits) - 1;      /* 30 digits */

    /* Convert idx_t to size_t for array indexing */
    if (n < 33) return 0;
    size_t idx = (size_t)(n - 33u);
    if (idx >= len) return 0;

    return (seq_t)(digits[idx] - '0');
}
