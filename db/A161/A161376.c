/*
 * A161376  Number of letters in Spanish words for months of the year.
 * https://oeis.org/A161376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   a(n) = a(n-12) for n>12; the first 12 terms are the number of
 *            letters in the Spanish names of the months:
 *            ["Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio",
 *            "Agosto","Septiembre","Octubre","Noviembre","Diciembre"]
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>

seq_t a(idx_t n)
{
    /* Array of month names in Spanish.  The array itself is not a
       lookup table for the sequence values; the lengths are computed
       at runtime from the strings. */
    static const char *const months[12] = {
        "Enero",      /* 5 */
        "Febrero",    /* 7 */
        "Marzo",      /* 5 */
        "Abril",      /* 5 */
        "Mayo",       /* 4 */
        "Junio",      /* 5 */
        "Julio",      /* 5 */
        "Agosto",     /* 6 */
        "Septiembre", /*10*/
        "Octubre",    /* 7 */
        "Noviembre",  /* 9 */
        "Diciembre"   /* 9 */
    };

    /* Periodicity of the sequence: 12 months repeat. */
    idx_t idx = (n - 1) % 12;

    /* Compute length of the month in a type‑agnostic way. */
    size_t len = strlen(months[idx]);

    return (seq_t)len;
}
