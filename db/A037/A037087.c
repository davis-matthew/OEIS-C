/*
 * A037087  Beatty sequence for e^(1/e).
 * https://oeis.org/A037087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n * e^(1/e)) We use a 20‑bit fixed‑point
 *            approximation of e^(1/e): α = floor(e^(1/e) * 2^20) = 1514844.
 *            Then a(n) = floor((n * α) / 2^20). For a 64‑bit implementation
 *            the product is performed with unsigned __int128 to avoid
 *            overflow; the code works for all builds because seq_t can be...
 * Range:     u8 n <= 177, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned long long ALPHA   = 1514844ULL;   /* floor(e^(1/e) * 2^20) */
static const unsigned int  SHIFT      = 20;             /* 2^SHIFT == 1048576 */
static const unsigned long long MASK      = ((1ULL << SHIFT) - 1ULL);

seq_t a(idx_t n)
{
    /* Split n into q*2^SHIFT + r to avoid overflow. */
    const unsigned long long q = n >> SHIFT;              /* n / 2^20 */
    const unsigned long long r = n & MASK;                /* n % 2^20 */

    /* term1 = q * ALPHA  (fits in 64 bits for all n whose a(n) fits) */
    const unsigned __int128 prod1 = (unsigned __int128)q * ALPHA;
    const unsigned long long term1 = (unsigned long long)prod1;

    /* term2 = floor((r * ALPHA) / 2^SHIFT) */
    const unsigned __int128 prod2 = (unsigned __int128)r * ALPHA;
    const unsigned long long term2 = (unsigned long long)(prod2 >> SHIFT);

    const unsigned long long result = term1 + term2;
    return (seq_t)result;
}
