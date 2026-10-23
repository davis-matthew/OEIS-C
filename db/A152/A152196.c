/*
 * A152196  A sequence based on the digits of Log(10) as a BBP:
 *          a(k)=Floor[Mod[(1/2)*((-4)^k)*(6/(4*k + 1) - 3/(4*k + 3) - 1/(4*k
 *          + 4)), 3]].
 * https://oeis.org/A152196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(k) = Floor[Mod[(1/2)*((-4)^k)*(6/(4*k + 1) - 3/(4*k + 3) -
 *            1/(4*k + 4)), 3]]
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* work with unsigned 64-bit intermediates; use 128-bit for safety */
    uint64_t k = (uint64_t)n;

    uint64_t d1 = 4*k + 1;
    uint64_t d2 = 4*k + 3;
    uint64_t d3 = 4*k + 4;

    /* D = d1*d2*d3, Q = 2*D, M = 3*Q  (all fit in 64 bits for k≤10000) */
    uint64_t D = (uint64_t)((__uint128_t)d1 * d2 * d3);
    uint64_t Q = 2 * D;
    uint64_t M = 3 * Q;          /* < 4·10^14, fits in 64 bits */

    /* N = 6*d2*d3 - 3*d1*d3 - d1*d2   (signed 64‑bit) */
    int64_t N = (int64_t)6 * (int64_t)d2 * (int64_t)d3
              - (int64_t)3 * (int64_t)d1 * (int64_t)d3
              - (int64_t)d1 * (int64_t)d2;

    /* N mod M, adjust for possible negative N */
    uint64_t Nmod = (uint64_t)((N % (int64_t)M + (int64_t)M) % (int64_t)M);

    /* Compute (-4)^k mod M via binary exponentiation */
    uint64_t base = (M + M - 4) % M;   /* M - 4   (non‑negative) */
    uint64_t exp  = k;
    uint64_t pow_mod = 1 % M;
    while (exp > 0) {
        if (exp & 1)
            pow_mod = (uint64_t)((__uint128_t)pow_mod * base % M);
        base = (uint64_t)((__uint128_t)base * base % M);
        exp >>= 1;
    }

    /* P mod M = (-4)^k * N mod M  */
    uint64_t Pmod = (uint64_t)((__uint128_t)pow_mod * Nmod % M);

    /* a(k) = floor(P/Q) mod 3 = floor( (P mod (3Q)) / Q ) */
    uint64_t a_val = Pmod / Q;   /* 0,1,2 */

    return (seq_t)a_val;
}
