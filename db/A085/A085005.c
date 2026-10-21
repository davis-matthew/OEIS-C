/*
 * A085005  A Von Koch curve related to the Golden ratio.
 * https://oeis.org/A085005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A085005: a(n) = 2*∑_{k=1}^n ∑_{i=1}^k b(i) – n*(n+1)/2,
   where b(k) = floor(φ*k) mod 2 and φ = (1+√5)/2. */

/* Integer square‑root for unsigned __int128.  Returns ⌊√x⌋. */
static unsigned long long isqrt_u128(unsigned __int128 x)
{
    unsigned long long low  = 0;
    unsigned long long high = 1ULL << 32;          /* high big enough for n≤10000 */

    /* Grow upper bound until high*high > x. */
    while ((unsigned __int128)high * high <= x) {
        low  = high;
        high <<= 1;
    }

    /* Binary search between low and high. */
    while (low < high) {
        unsigned long long mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t cumulative = 0;      /* Σ_{i=1}^k b(i)  */
    seq_t sum_c      = 0;      /* Σ_{k=1}^n cumulative */

    for (idx_t k = 1; k <= n; ++k) {
        /* m = ⌊√(5*k²)⌋  */
        unsigned __int128 prod = (unsigned __int128)5 * k * k;
        unsigned long long m   = isqrt_u128(prod);

        /* floor(φ*k) = (k + m) / 2  */
        seq_t floor_phi = (((seq_t)k) + (seq_t)m) >> 1;

        /* b(k) = floor(φ*k) mod 2 */
        seq_t b = floor_phi & (seq_t)1;

        cumulative = cumulative + b;
        sum_c      = sum_c + cumulative;
    }

    seq_t tn         = (seq_t)n;
    seq_t triangular = tn * (tn + (seq_t)1) / (seq_t)2;
    seq_t result     = sum_c + sum_c - triangular;
    return result;
}
