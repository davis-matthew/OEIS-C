/*
 * A073094  Final digit of C(2k,k) when not equal to zero.
 * https://oeis.org/A073094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = C(2*A037453(n), A037453(n)) mod 10 (last non‑zero digit),
 *            where A037453(n) is obtained by interpreting n in base‑3 as a
 *            base‑5 number. The last digit is the even digit d∈{2,4,6,8}
 *            satisfying d≡2^k (mod 5) with k=number of 1’s in the base‑3
 *            expansion of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned char pow2mod5[4] = {1, 2, 4, 3};
static const unsigned char last_digit[5] = {0, 6, 2, 8, 4};

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;                 /* undefined for n<1, return safe value */

    idx_t  t     = n;
    unsigned cnt1 = 0;

    while (t)
    {
        unsigned d = t % 3;
        if (d == 1)
            ++cnt1;
        t /= 3;
    }

    unsigned char r = pow2mod5[cnt1 & 3];   /* 2^cnt1 mod 5   */
    return (seq_t)last_digit[r];            /* map to last non‑zero digit */
}
