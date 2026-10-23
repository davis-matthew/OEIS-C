/*
 * A115182  Self-inverse integer permutation induced by Beatty sequences for
 *          CC and Cc/(Cc-1) with Cc = 1.234567891011121314... =
 *          10*(Champernowne constant).
 * https://oeis.org/A115182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(A115180(n)) = A115181(n) and a(A115181(n)) = A115180(n)
 * Range:     u8 n <= 59, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Constants for the Beatty sequences
       Denominator (10^15) and scaled numerators
       r = 1.234567891011121314...   (Cc)
       s = r / (r-1)                 = r/(Cc-1)
       R_INT = floor(r * 10^15)      = 1234567891011121
       R_M1_INT = floor((r-1) * 10^15) = 234567891011121   */
    const unsigned long long DEN     = 1000000000000000ULL;        /* 10^15 */
    const unsigned long long R_INT   = 1234567891011121ULL;        /* r * 10^15  */
    const unsigned long long R_M1_INT = 234567891011121ULL;        /* (r-1) * 10^15 */

    unsigned long long m = n;                /* work on a uint64_t copy */

    /* Find the Beatty index ℓ such that m ≈ floor(ℓ * r) or floor(ℓ * s)   */
    unsigned __int128 prod1 = (unsigned __int128)m * DEN;
    unsigned long long   k   = (unsigned long long)(prod1 / R_INT); /* floor(m/r) */
    unsigned long long   l   = k + 1;                              /* candidate ℓ */

    /* Test whether m belongs to the first Beatty sequence A115180 (floor(ℓ * r)) */
    unsigned __int128 prod2 = (unsigned __int128)l * R_INT;
    unsigned long long   val_r = (unsigned long long)(prod2 / DEN);

    if (val_r == m) {
        /* m is in A115180, partner is floor(ℓ * s) = floor(ℓ * R_INT / R_M1_INT) */
        unsigned __int128 prod3 = (unsigned __int128)l * R_INT;
        unsigned long long partner = (unsigned long long)(prod3 / R_M1_INT);
        return (seq_t)partner;
    } else {
        /* m is in A115181, index ℓ_s = floor(m / s) = floor(m * R_M1_INT / R_INT) */
        unsigned __int128 prod4 = (unsigned __int128)m * R_M1_INT;
        unsigned long long   l_s = (unsigned long long)(prod4 / R_INT);

        /* partner is floor((ℓ_s + 1) * r) */
        unsigned __int128 prod5 = (unsigned __int128)(l_s + 1) * R_INT;
        unsigned long long partner = (unsigned long long)(prod5 / DEN);
        return (seq_t)partner;
    }
}
