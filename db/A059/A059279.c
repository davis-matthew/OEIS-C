/*
 * A059279  G.f. is ((1-x)/(1-2*x)) * G(x*(1-x)/(1-2*x)) where G(x) is g.f.
 *          for Catalan numbers A000108.
 * https://oeis.org/A059279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*(4*n-1)*a(n-1) - 4*(4*n-5)*a(n-2) + 4*(2*n-5)*a(n-3))
 *            / (n+1), * with a(0)=1, a(1)=2, a(2)=6
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>
#include <stdint.h>

/* The build supplies seq_t (value type) and idx_t (index type). */
/* We rely on seq_t being either an unsigned integer type (uint8_t … uint64_t,
 * unsigned __int128) or an arbitrary-precision integer class.           */

static seq_t *table = NULL;      /* memoisation table */
static idx_t  table_size = 0;    /* number of entries stored */

#ifdef __cplusplus
#include <type_traits>
#endif

/* Compute a(n) ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Allocate initial table with the first three known terms. */
    if (table == NULL) {
        table_size = 3;
        table = (seq_t *)malloc(table_size * sizeof(seq_t));
        if (!table) return 0;                       /* unlikely */
        table[0] = (seq_t)1;
        table[1] = (seq_t)2;
        table[2] = (seq_t)6;
    }

    /* If requested value already known, return it. */
    if (n < table_size)
        return table[n];

    /* Extend the table to hold up to index n. */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(table, new_size * sizeof(seq_t));
    if (!tmp) return 0;                           /* out of memory */
    table = tmp;

    /* Compute missing terms. */
    for (idx_t i = table_size; i <= n; ++i) {
        /* Bounds must fit in seq_t; cast where appropriate. */
        if (i == 0) { table[i] = (seq_t)1; continue; }
        if (i == 1) { table[i] = (seq_t)2; continue; }
        if (i == 2) { table[i] = (seq_t)6; continue; }

        /* Pre‑fetch the three previous values. */
        seq_t a1 = table[i-1];
        seq_t a2 = table[i-2];
        seq_t a3 = table[i-3];

#if defined(__cplusplus) && (defined(__cpp_if_constexpr) || defined(__cplusplus) && (__cplusplus >= 201703L))
        /* Compile‑time branch: integral seq_t or arbitrary‑precision class. */
        if constexpr (std::is_integral<seq_t>::value && std::is_unsigned<seq_t>::value) {
            /* Use unsigned __int128 for intermediate calculations. */
            unsigned __int128 mul4i1 = (unsigned __int128)4 * i - 1;
            unsigned __int128 mul4i5 = (unsigned __int128)4 * i - 5;
            unsigned __int128 mul2i5 = (unsigned __int128)2 * i - 5;

            unsigned __int128 term1 = (unsigned __int128)2 * mul4i1 * (unsigned __int128)a1;
            unsigned __int128 term2 = (unsigned __int128)4 * mul4i5 * (unsigned __int128)a2;
            unsigned __int128 term3 = (unsigned __int128)4 * mul2i5 * (unsigned __int128)a3;

            unsigned __int128 num = term1 - term2 + term3;
            unsigned __int128 denom = (unsigned __int128)(i + 1);

            table[i] = (seq_t)(num / denom);
        } else {
            /* seq_t is an arbitrary‑precision class: rely on its operators. */
            seq_t coeff1 = (seq_t)2 * (seq_t)((seq_t)4 * (seq_t)i - (seq_t)1);
            seq_t coeff2 = (seq_t)4 * (seq_t)((seq_t)4 * (seq_t)i - (seq_t)5);
            seq_t coeff3 = (seq_t)4 * (seq_t)((seq_t)2 * (seq_t)i - (seq_t)5);

            seq_t num = coeff1 * a1 - coeff2 * a2 + coeff3 * a3;
            table[i] = num / (seq_t)(i + 1);
        }
#else
        /* C compilation: assume seq_t is an unsigned integer type.  */
        unsigned __int128 mul4i1 = (unsigned __int128)4 * i - 1;
        unsigned __int128 mul4i5 = (unsigned __int128)4 * i - 5;
        unsigned __int128 mul2i5 = (unsigned __int128)2 * i - 5;

        unsigned __int128 term1 = (unsigned __int128)2 * mul4i1 * (unsigned __int128)a1;
        unsigned __int128 term2 = (unsigned __int128)4 * mul4i5 * (unsigned __int128)a2;
        unsigned __int128 term3 = (unsigned __int128)4 * mul2i5 * (unsigned __int128)a3;

        unsigned __int128 num = term1 - term2 + term3;
        unsigned __int128 denom = (unsigned __int128)(i + 1);

        table[i] = (seq_t)(num / denom);
#endif
    }

    table_size = new_size;
    return table[n];
}
