/*
 * A022924  Number of numbers of the form 3^m between 2^n and 2^(n+1).
 * https://oeis.org/A022924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)*β) – floor(n*β) where β = log₃(2) ≈
 *            0.6309297535714574. This counts how many powers of 3 lie
 *            strictly between 2ⁿ and 2ⁿ⁺¹
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Rational approximation of β = log₃(2) with a relative error < 10⁻¹⁶. */
    static const unsigned long long NUM = 630929753571457ULL;   /* numerator   */
    static const unsigned long long DEN = 1000000000000000ULL;   /* denominator */

    /* Compute floor(n*β) and floor((n+1)*β) using integer arithmetic. */
    unsigned __int128 prod_n   = (unsigned __int128)n   * NUM;
    unsigned __int128 prod_np1 = (unsigned __int128)(n + 1) * NUM;

    /* The difference is 0 or 1, safe to cast to seq_t. */
    return (seq_t)(prod_np1 / DEN - prod_n / DEN);
}
