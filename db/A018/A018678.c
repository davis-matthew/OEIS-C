/*
 * A018678  Divisors of 845.
 * https://oeis.org/A018678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A018678: Divisors of 845.
   Enumerate all divisors of 845 (prime factorization 5¹·13²)
   in ascending order and return the nth element (1‑based).  */
seq_t a(idx_t n)
{
    /* only 6 divisors for 845 */
    const unsigned SMALL_DIV_CNT = 6U;
    seq_t divisors[SMALL_DIV_CNT];

    /* generate all divisors via exponent loops */
    unsigned pos = 0U;
    for (unsigned e5 = 0U; e5 <= 1U; ++e5) {
        seq_t base5 = 1;
        for (unsigned i = 0U; i < e5; ++i) base5 *= (seq_t)5;
        for (unsigned e13 = 0U; e13 <= 2U; ++e13) {
            seq_t val = base5;
            for (unsigned j = 0U; j < e13; ++j) val *= (seq_t)13;
            divisors[pos++] = val;
        }
    }

    /* sort the array in ascending order (simple insertion sort) */
    for (unsigned i = 1U; i < SMALL_DIV_CNT; ++i) {
        seq_t key = divisors[i];
        unsigned j = i;
        while (j > 0U && divisors[j-1] > key) {
            divisors[j] = divisors[j-1];
            --j;
        }
        divisors[j] = key;
    }

    /* return the requested element, 0 if n out of range */
    if (n == 0U) return (seq_t)0;
    if (n <= SMALL_DIV_CNT)
        return divisors[(unsigned)(n - 1U)];
    return (seq_t)0;
}
