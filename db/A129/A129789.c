/*
 * A129789  a(n) = ceiling(5^n/n).
 * https://oeis.org/A129789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(5^n / n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>     /* for NULL etc. */

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* defined only for n >= 1 */

    /* ---------- 128‑bit intermediate for small seq_t types  ---------- */
    #ifdef __SIZEOF_INT128__
    /* Use the 128‑bit type only when seq_t fits in 64 bits or fewer. */
    if (sizeof(seq_t) <= 8) {
        __int128 val  = 1;      /* 5^n */
        __int128 base = 5;
        idx_t exp = n;
        while (exp) {
            if (exp & 1) val *= base;
            base *= base;
            exp >>= 1;
        }
        /* ceil(5^n / n) = (val + n - 1) / n, but avoid overflow by using division and modulo. */
        __int128 q = val / n;
        if (val % n != 0) q += 1;
        return (seq_t)q;
    }
    #endif

    /* ---------- Generic path using seq_t operations (handles big‑int) --- */
    seq_t val  = 1;
    seq_t base = 5;
    idx_t exp = n;
    while (exp) {
        if (exp & 1) val = val * base;
        base = base * base;
        exp >>= 1;
    }
    seq_t quotient = val / n;   /* floor division */
    seq_t remainder = val % n;
    if (remainder > 0) quotient += 1;   /* ceil adjustment */
    return quotient;
}
