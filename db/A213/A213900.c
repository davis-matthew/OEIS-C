/*
 * A213900  The minimum number of 11's in the relation n*[n,11,11,...,11,n] =
 *          [x,...,x] between simple terminating continued fractions.
 * https://oeis.org/A213900
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = z_U(n)-1 for n≥2 U(0)=0, U(1)=1, U(k+2)=11*U(k+1)+U(k),
 *            z_U(n)=min{k≥1 : n divides U(k)}
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* undefined for n<2 */

    /* All arithmetic is performed modulo n using idx_t to keep values small. */
    idx_t mod = n;
    idx_t a_mod = 1 % mod;               /* U(1) mod n */
    idx_t b_mod = 11 % mod;              /* U(2) mod n */

    /* If U(2) ≡ 0 (mod n), then z_U(n)=2, hence a(n)=1. */
    if (b_mod == 0) return (seq_t)1;

    idx_t k = 2;                         /* current index (U(k) mod n is b_mod) */

    while (b_mod != 0) {
        /* Compute U(k+1) = 11*U(k) + U(k-1) modulo mod.
           Use 128‑bit intermediate to avoid uint64_t overflow. */
        unsigned __int128 tmp = (unsigned __int128)11 * b_mod + a_mod;
        idx_t next = (idx_t)(tmp % mod);

        a_mod = b_mod;
        b_mod = next;
        ++k;
    }

    /* k = z_U(n), so a(n) = k-1. */
    return (seq_t)(k - 1);
}
