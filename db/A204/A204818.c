/*
 * A204818  Final nonzero digit of n^n in base 8.
 * https://oeis.org/A204818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ( ( (n >> v2(n)) ^ n ) * 2^((v2(n)*n) % 3) ) mod 8
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* v2(n) = exponent of 2 in the factorisation of n */

/* compute the final non‑zero digit of n^n in base 8 */
seq_t a(idx_t n)
{
    if (n == 0)                /* 0^0 is defined to be 1 in this sequence */
        return (seq_t)1;

    /* compute v2(n) by counting trailing zero bits */
    idx_t v     = 0;
    idx_t temp  = n;
    while ((temp & 1) == 0) {
        ++v;
        temp >>= 1;
    }

    /* e_rem = (v * n) mod 3  (0 ≤ e_rem ≤ 2) */
    idx_t e_rem = (v * n) % 3;

    /* odd part of n (n with all factors of 2 removed) */
    idx_t odd = n >> v;

    /* odd part mod 8 (always odd because 2 factors removed) */
    unsigned odd_mod8 = (unsigned)(odd & 7ULL);

    /* odd_part^n mod 8: for odd base, square ≡ 1 (mod 8), so
       the result is the base itself if n is odd, otherwise 1   */
    unsigned odd_pow_mod8 = ((n & 1) == 0) ? 1u : odd_mod8;

    /* multiply by 2^e_rem (e_rem ∈ {0,1,2}) and reduce modulo 8 */
    unsigned mult   = 1u << e_rem;          /* 1, 2, or 4 */
    unsigned result = (odd_pow_mod8 * mult) & 7u;  /* modulo 8 */

    return (seq_t)result;
}
