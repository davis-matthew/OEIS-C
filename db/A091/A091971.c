/*
 * A091971  G.f.: (1+x^3)*(1+x^5)*(1+x^6)/((1-x^4)*(1-x^5)*(1-x^6)).
 * https://oeis.org/A091971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1) = 0 a(2) = 0 a(3) = 1 a(4) = 1 a(5) = 2 a(6) = 2
 *            a(7) = 1 a(8) = 3 a(9) = 4 a(10)= 4 a(11)= 5 a(12)= 5 a(13)= 6
 *            a(14)= 8 a(15)= 9 a(16)= 9 a(17)=10 a(18)=12 a(19)=13 a(20)=15
 *            a(21)=16 a(22)=16 a(23)=19 In general the sequence satisfies the
 *            linear recurrence derived from the generati...
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */

#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *a_cache = NULL;   /* memoised sequence values          */
static size_t  a_capacity = 0;  /* allocated size of a_cache         */
static size_t  a_len      = 0;  /* next free index (last computed+1) */

/* coefficients of the numerator N(x) = 1 + x^3 + x^5 + x^6 + x^8 + x^9 + x^11 + x^14 */
static const int Ncoeff[15] = {
    1,0,0,1,0,1,1,0,1,1,0,1,0,0,1
};

/* grow the cache so that it contains terms up to and including index n */
static void grow_to(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (a_capacity < needed) {
        size_t new_cap = a_capacity ? a_capacity * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *new_a = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!new_a) abort();                 /* allocation failed */
        a_cache = new_a;
        a_capacity = new_cap;
    }

    for (size_t i = a_len; i <= (size_t)n; ++i) {
        seq_t term = 0;
        if (i < 15) term = (seq_t)Ncoeff[i];

        /* positive contributions */
        if (i >= 4) term += a_cache[i-4];
        if (i >= 5) term += a_cache[i-5];
        if (i >= 6) term += a_cache[i-6];
        if (i >= 15) term += a_cache[i-15];

        /* negative contributions */
        if (i >= 9)  term -= a_cache[i-9];
        if (i >= 10) term -= a_cache[i-10];
        if (i >= 11) term -= a_cache[i-11];

        a_cache[i] = term;
    }
    a_len = needed;
}

/* Public sequence function */
extern seq_t a(idx_t n)
{
    grow_to(n);
    return a_cache[(size_t)n];
}
