/*
 * A100833  Smallest positive palindrome-free and squarefree sequence.
 * https://oeis.org/A100833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n ≡ 1 (mod 3); 2 if n ≡ 2 (mod 3); 3 + v2(n/3) if n
 *            ≡ 0 (mod 3). Here v2(m) is the exponent of the highest power of
 *            2 dividing m
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  n may be large, use idx_t operations for indices */
    idx_t mod3 = n % 3;
    if (mod3 == 1)
        return (seq_t)1;
    if (mod3 == 2)
        return (seq_t)2;

    /* n ≡ 0 (mod 3) */
    idx_t k = n / 3;            /* k ≥ 1 because n ≥ 3 here */
    /* Count trailing zeros of k, i.e., v2(k) */
    int cnt = 0;
    while ((k & 1) == 0)        /* k is even; division by 2 */
    {
        cnt++;
        k >>= 1;
    }
    return (seq_t)(3 + cnt);
}
