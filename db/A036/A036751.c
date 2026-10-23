/*
 * A036751  Number of strings of n distinct digits from 0-9 that are the last
 *          n digits of a square in base 8.
 * https://oeis.org/A036751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of length‑n strings of distinct octal digits that are
   the last n digits of an integer square (i.e. are quadratic residues
   modulo 8^n).  For n > 8 the value is 0 because only 8 distinct
   octal digits exist. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* zero or negative indices are undefined; return 0 */
    if (n < 1)
        return (seq_t)0;

    /* For n > 8 distinct digits cannot be formed in base 8 */
    if (n > 8)
        return (seq_t)0;

    /* Compute modulus 8^n = 1 << (3*n) */
    const idx_t pw = 3 * n;
    const idx_t mod = (idx_t)1 << pw;          /* mod <= 2^24 = 16,777,216 */

    /* Boolean array marking which residues are quadratic residues.
       Size: mod bytes (mod <= 16M). */
    unsigned char *sq = (unsigned char *)calloc((size_t)mod, 1);
    if (!sq)
        return (seq_t)0;                      /* allocation failure → 0 */

    /* Mark all square residues k^2 modulo 8^n. */
    for (idx_t k = 0; k < mod; ++k) {
        const idx_t r = (k * k) & (mod - 1);   /* power‑of‑two modulus */
        sq[(size_t)r] = 1;
    }

    /* Count residues that are squares and have all distinct base‑8 digits. */
    seq_t count = 0;
    for (idx_t r = 0; r < mod; ++r) {
        if (!sq[(size_t)r])
            continue;                         /* not a square residue */

        idx_t val = r;
        unsigned used = 0;
        bool distinct = true;
        for (unsigned pos = 0; pos < (unsigned)n; ++pos) {
            unsigned digit = val & 7;          /* current octal digit */
            if (used & (1u << digit)) {        /* digit already used */
                distinct = false;
                break;
            }
            used |= 1u << digit;
            val >>= 3;                         /* next digit */
        }
        if (distinct)
            ++count;
    }

    free(sq);
    return count;
}
