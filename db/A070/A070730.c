/*
 * A070730  Euro coinage (in cents) from smallest physical size to largest.
 * https://oeis.org/A070730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = value (in cents) of the euro coin with the n‑th smallest
 *            * physical diameter. The sequence is obtained by ordering the
 *            eight * euro coins by their diameters * 16.25 mm, 18.50 mm,
 *            19.75 mm, 21.25 mm, 22.50 mm, 23.25 mm, 24.25 mm, * 25.75 mm,
 *            and taking their cent values 1, 2, 10, 5, 20, 100...
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Data structure for one coin.  Diameter stored as tenths of a millimetre
 * (i.e. multiples of 0.01 mm) to keep it integral.  Value is the
 * amount in cents.  The sequence contains only these eight coins. */
struct coin {
    unsigned int diameter; /* 0.01 mm units */
    seq_t       value;     /* value in cents */
};

/* Comparator used to sort the coins by increasing diameter. */
static int cmp_coin(const void *p1, const void *p2) {
    const struct coin *c1 = (const struct coin *)p1;
    const struct coin *c2 = (const struct coin *)p2;
    return (c1->diameter < c2->diameter) ? -1 :
           (c1->diameter > c2->diameter) ?  1 : 0;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Unsorted list of all euro coins.  The order here is not important
     * for correctness; we sort the list once on the first call. */
    static const struct coin unsorted[] = {
        { 1625U,  (seq_t)1 },   /* 1 cent  (16.25 mm) */
        { 1850U,  (seq_t)2 },   /* 2 cents (18.50 mm) */
        { 1975U,  (seq_t)10 },  /* 10 cents(19.75 mm) */
        { 2125U,  (seq_t)5 },   /* 5 cents (21.25 mm) */
        { 2250U,  (seq_t)20 },  /* 20 cents(22.50 mm) */
        { 2325U,  (seq_t)100 }, /* 1 euro  (23.25 mm) */
        { 2425U,  (seq_t)50 },  /* 50 cents(24.25 mm) */
        { 2575U,  (seq_t)200 }  /* 2 euros (25.75 mm) */
    };
    /* Sorted list will be built once. */
    static struct coin sorted[8];
    static int initialized = 0;

    if (!initialized) {
        /* Copy to the mutable array and sort it. */
        for (size_t i = 0; i < 8; ++i) {
            sorted[i] = unsorted[i];
        }
        qsort(sorted, 8, sizeof(struct coin), cmp_coin);
        initialized = 1;
    }

    /* If the requested index is within bounds, return the value; otherwise
     * the sequence is finite and all other terms are defined to be zero. */
    if ((size_t)n < 8U) {
        return sorted[(size_t)n].value;
    } else {
        return (seq_t)0;
    }
}
