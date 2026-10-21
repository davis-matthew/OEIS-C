/*
 * A086415  Maximal exponent in prime factorization of 3-smooth numbers.
 * https://oeis.org/A086415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum exponent in the prime factorization of the n-th
 *            3‑smooth number (numbers of the form 2^i 3^j). The function
 *            generates the smooth numbers in increasing order by merging the
 *            sequences obtained by multiplying previous numbers by 2 and by
 *            3. Duplicate values (which may occur when 2^(i+...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Return 0 for any index below the offset.  The sequence is defined
       for n >= 1. */
    if (n < 1) return (seq_t)0;

    /* Static state retained between calls. */
    static idx_t cur_max = 0;          /* number of entries already generated, 1‑based */
    static idx_t p2 = 1, p3 = 1;       /* pointers into the list for the next *2 and *3 */
    static size_t capacity = 0;        /* size allocated for the two exponent arrays */
    static uint16_t *exp_i = NULL;     /* exponent of 2 in the i‑th 3‑smooth number */
    static uint16_t *exp_j = NULL;     /* exponent of 3 in the i‑th 3‑smooth number */

    /* Ensure we have space for the requested index. */
    if ((size_t)(n + 1) > capacity) {
        size_t new_cap = (n + 1) * 2;
        uint16_t *tmp_i = (uint16_t *)realloc(exp_i, new_cap * sizeof(uint16_t));
        uint16_t *tmp_j = (uint16_t *)realloc(exp_j, new_cap * sizeof(uint16_t));
        if (!tmp_i || !tmp_j) { /* allocation failure – abort in this simple implementation */
            exit(EXIT_FAILURE);
        }
        exp_i = tmp_i;
        exp_j = tmp_j;
        capacity = new_cap;
    }

    /* Initialise the list for the first call. */
    if (cur_max == 0) {
        /* 1 = 2^0 * 3^0 */
        exp_i[1] = 0;
        exp_j[1] = 0;
        cur_max  = 1;
        p2 = 1;
        p3 = 1;
    }

    /* Pre-computed logarithms for ordering. */
    const double ln2 = 0.693147180559945309417232121458;  /* ln(2) */
    const double ln3 = 1.09861228866810969139524523692;   /* ln(3) */

    /* Generate further 3‑smooth numbers up to the requested index. */
    while (cur_max < n) {
        /* Candidates obtained by multiplying the numbers at positions p2
           and p3 by 2 and 3 respectively. */
        uint32_t i2 = (uint32_t)exp_i[p2] + 1;   /* exponent of 2 after multiplying by 2 */
        uint32_t j2 = (uint32_t)exp_j[p2];       /* exponent of 3 unchanged */
        uint32_t i3 = (uint32_t)exp_i[p3];       /* exponent of 2 unchanged */
        uint32_t j3 = (uint32_t)exp_j[p3] + 1;   /* exponent of 3 after multiplying by 3 */

        /* Detect equality exactly using integer arithmetic. */
        bool equal = (i2 == i3) && (j2 == j3);

        uint16_t chosen_i, chosen_j;

        if (equal) {
            /* Both candidates are the same.  Take one and advance both pointers. */
            chosen_i = (uint16_t)i2;
            chosen_j = (uint16_t)j2;
            ++p2;
            ++p3;
        } else {
            /* Compare the logarithms to decide which candidate is smaller. */
            double val2 = i2 * ln2 + j2 * ln3;
            double val3 = i3 * ln2 + j3 * ln3;

            if (val2 < val3) {
                chosen_i = (uint16_t)i2;
                chosen_j = (uint16_t)j2;
                ++p2;
            } else {
                chosen_i = (uint16_t)i3;
                chosen_j = (uint16_t)j3;
                ++p3;
            }
        }

        ++cur_max;
        exp_i[cur_max] = chosen_i;
        exp_j[cur_max] = chosen_j;
    }

    /* The answer for index n is the larger of the two exponents. */
    seq_t max_exp = (exp_i[n] > exp_j[n]) ? exp_i[n] : exp_j[n];
    return max_exp;
}
