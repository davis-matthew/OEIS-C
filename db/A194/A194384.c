/*
 * A194384  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) > 0, where
 *          r=sqrt(8) and < > denotes fractional part.
 * https://oeis.org/A194384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer m with * Sum_{k=1..m} ( <1/2 +
 *            k*sqrt(8)> – <k*sqrt(8)> ) > 0 * This sum is >0 iff the number
 *            of k≤m with {k*sqrt(8)} < 1/2 * exceeds m/2. We test each m
 *            sequentially and count how many * satisfy the inequality. The m
 *            that succeed are the sequence
 * Range:     u8 n <= 32, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square‑root for 128‑bit unsigned values.
   Returns floor(sqrt(n)).  The algorithm works for all n < 2^128. */
static unsigned long long isqrt_u128(unsigned __int128 n)
{
    unsigned __int128 x = 0, bit;
    /* largest power of four ≤ n */
    bit = ((unsigned __int128)1) << 126;
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= x + bit) {
            n -= x + bit;
            x = (x >> 1) + bit;
        } else {
            x >>= 1;
        }
        bit >>= 2;
    }
    return (unsigned long long)x;
}

/* floor(k*sqrt(8)) as an unsigned long long */
static inline unsigned long long floor_sqrt8(idx_t k)
{
    unsigned __int128 v = (unsigned __int128)8 * (unsigned __int128)k * (unsigned __int128)k;
    return isqrt_u128(v);
}

/* floor(k*sqrt(32)) = floor(2*k*sqrt(8)) as an unsigned long long */
static inline unsigned long long floor_sqrt32(idx_t k)
{
    unsigned __int128 v = (unsigned __int128)32 * (unsigned __int128)k * (unsigned __int128)k;
    return isqrt_u128(v);
}

seq_t a(idx_t n)
{
    idx_t m = 0;                      /* current candidate m   */
    idx_t found = 0;                  /* how many terms found  */
    unsigned long long high = 0;      /* count of {k*sqrt8}≥½ */

    while (found < n) {
        ++m;
        unsigned long long a_val = floor_sqrt8(m);
        unsigned long long c_val = floor_sqrt32(m);
        /* c_val == 2*a_val+1  iff fractional part of k*sqrt(8) >= ½ */
        if (c_val == 2 * a_val + 1)
            ++high;
        /* S(m) > 0  <=>  2*high < m  (high = count of ≥½) */
        if (2 * high < (unsigned long long)m) {
            ++found;
            if (found == n)
                return (seq_t)m;
        }
    }
    /* never reached – keeps compiler happy */
    return (seq_t)0;
}
