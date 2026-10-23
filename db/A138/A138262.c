/*
 * A138262  Smallest positive integer m such that n divides [9^m/m]
 *          (=A129799(m)).
 * https://oeis.org/A138262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A138262:  a(n) = smallest positive integer m such that
 *  n | ⌊9^m/m⌋
 *
 *  Algorithm: for each m compute
 *      A = 9^m (mod m*n)
 *      B = 9^m (mod m)
 *  Then ⌊9^m/m⌋ ≡ (A-B)/m (mod n).
 *  Since 0 ≤ (A-B)/m < n, the residue is 0 iff (A-B)/m == 0.
 */

#include <stdint.h>
#include <assert.h>

/* Helper: modular exponentiation with 128‑bit intermediate multiplication */
static inline seq_t mod_pow(uint64_t base, idx_t exp, seq_t mod)
{
    if (mod <= 1) return 0;
    seq_t result = 1;
    seq_t b = (seq_t)(base % mod);
    while (exp > 0) {
        if (exp & 1)
            result = (seq_t)(((__int128)result * b) % mod);
        b = (seq_t)(((__int128)b * b) % mod);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* the offset in OEIS starts at 1 */

    for (idx_t m = 1; ; ++m) {
        /* m and n fit into idx_t; product fits into 128‑bit */
        seq_t mod_nm = (seq_t)n * (seq_t)m;

        seq_t A = mod_pow(9, m, mod_nm);          /* 9^m (mod m*n) */
        seq_t B = mod_pow(9, m, (seq_t)m);        /* 9^m (mod m) */

        /* A ≡ B (mod m) ⇒ A-B is a multiple of m */
        seq_t diff = A - B;                       /* 0 <= diff < m*n */
        seq_t Q = diff / m;                       /* 0 <= Q < n */

        if (Q == 0)                 /* n divides ⌊9^m/m⌋ */
            return (seq_t)m;
    }
}
