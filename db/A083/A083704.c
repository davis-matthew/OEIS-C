/*
 * A083704  a(n)=sum(k=1,n,floor(r*floor(k/r))) where r=sqrt(3).
 * https://oeis.org/A083704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n} floor( sqrt(3) * floor(k / sqrt(3)) )
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for 64‑bit values (floor(sqrt(x))) */
static uint64_t isqrt_u64(uint64_t x) {
    uint64_t op = x;
    uint64_t res = 0;
    uint64_t one = 1ULL << 62;          /* the highest power of four <= op */
    while (one > op) one >>= 2;
    while (one != 0) {
        if (op >= res + one) {
            op -= res + one;
            res = (res >> 1) + one;
        } else {
            res >>= 1;
        }
        one >>= 2;
    }
    return res;
}

/* OEIS A083704 */
seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        uint64_t k_sq = (uint64_t)k * (uint64_t)k;          /* k^2 */
        uint64_t m = isqrt_u64(k_sq / 3);                  /* floor(k / sqrt(3)) */
        uint64_t t = isqrt_u64(3 * m * m);                /* floor(sqrt(3) * m) */
        sum += (seq_t)t;
    }
    return sum;
}
