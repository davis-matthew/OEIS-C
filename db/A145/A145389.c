/*
 * A145389  Digital roots of triangular numbers.
 * https://oeis.org/A145389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digital_root( n*(n+1)/2 ) (i.e. a(n) = 1 + ((n^2 + n -
 *            2)/2) mod 9 for n>0, a(0)=0)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset term a(0)=0 */
    if (n == 0) return (seq_t)0;

    /* Compute (n*(n+1)/2) mod 9 using modular arithmetic
       to avoid any overflow, even for very large n. */
    /* 2 has inverse 5 modulo 9 (since 2*5≡1 (mod 9)) */
    uint64_t n_mod   = n % 9;          /* n          mod 9 */
    uint64_t n1_mod  = (n + 1) % 9;    /* (n+1)      mod 9 */
    uint64_t t_mod   = (n_mod * n1_mod) % 9; /* n*(n+1)   mod 9 */
    t_mod = (t_mod * 5) % 9;              /* /2  mod 9 */

    /* Digital root of a positive integer x is:
       9 if x mod 9 == 0, otherwise x mod 9. */
    if (t_mod == 0)
        return (seq_t)9;
    else
        return (seq_t)t_mod;
}
