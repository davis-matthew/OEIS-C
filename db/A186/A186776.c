/*
 * A186776  Stanley Sequence S(0,2).
 * https://oeis.org/A186776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th smallest integer whose base‑3 representation
 *            contains only the digits 0 and 1, except that the least
 *            significant digit may be either 0 or 2. These numbers are
 *            exactly the union of X_m = φ(2m) (m ≥ 0) Y_m = φ(2m) + 2 (m ≥ 0)
 *            where φ(k) = Σ (k's binary bits)·3^i. The routine merges...
 * Range:     u8 n <= 39, u16 n <= 1280, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* φ(k) = sum over binary digits of k of 3^i  */
static inline uint64_t phi_uint64(idx_t k)
{
    uint64_t value = 0;
    uint64_t pow   = 1;        /* 3^i  */
    while (k != 0) {
        if (k & 1) {
            value += pow;
        }
        pow *= 3;
        k >>= 1;
    }
    return value;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* offset is 1, but guard against 0 */
    }

    idx_t count = 0;
    idx_t i = 0;     /* index for X_m = φ(2i)     */
    idx_t j = 0;     /* index for Y_m = φ(2j)+2   */

    uint64_t val_i, val_j, ret64;

    while (count < n) {
        val_i = phi_uint64(2 * i);    /* X_i */
        val_j = phi_uint64(2 * j) + 2;/* Y_j */

        if (val_i < val_j) {
            ret64 = val_i;
            ++i;
        } else if (val_j < val_i) {
            ret64 = val_j;
            ++j;
        } else {  /* equal, skip duplicate */
            ret64 = val_i;
            ++i;
            ++j;
        }
        ++count;
    }

    return (seq_t)ret64;
}
