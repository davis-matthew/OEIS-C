/*
 * A013463  Expansion of e.g.f. sin(arctan(x) - arctanh(x)) (odd powers only).
 * https://oeis.org/A013463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 10, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Recurrence:
   a(k+4) = -(16*(2k+7)*(2k+5)*(2k+3)*(2k+1)*(k+4)*(k+2)*(k+1)^2*a(k)
            +16*(k+2)*(2k+5)*(2k+7)*(k+4)*a(k+1)
            -8*(2k+7)*(2k+5)*(k+2)*(k+4)*a(k+2));
   Initial values: a(0)=0, a(1)=-4, a(2)=0, a(3)=-1440 */

seq_t a(idx_t n)
{
    static seq_t *mem = NULL;
    static size_t cap = 0;
    static size_t size = 0;
    static bool initialized = false;

    /* Initial seed values */
    const long init_vals[4] = {0, -4, 0, -1440};

    if (!initialized) {
        cap = 16;
        mem = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!mem) abort();
        for (size_t i = 0; i < 4; ++i)
            mem[i] = (seq_t)init_vals[i];
        size = 4;
        initialized = true;
    }

    /* Compute terms up to n */
    while (size <= (size_t)n) {
        if (size >= cap) {
            size_t new_cap = cap * 2;
            if (new_cap < size + 8)
                new_cap = size + 8;
            seq_t *tmp = (seq_t *)realloc(mem, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            mem = tmp;
            cap = new_cap;
        }

        idx_t k = (idx_t)(size - 4);

        seq_t term1 = mem[k];
        term1 *= (seq_t)16;
        term1 *= (seq_t)(2*k + 7);
        term1 *= (seq_t)(2*k + 5);
        term1 *= (seq_t)(2*k + 3);
        term1 *= (seq_t)(2*k + 1);
        term1 *= (seq_t)(k + 4);
        term1 *= (seq_t)(k + 2);
        term1 *= (seq_t)(k + 1);
        term1 *= (seq_t)(k + 1);

        seq_t term2 = mem[k + 1];
        term2 *= (seq_t)16;
        term2 *= (seq_t)(k + 2);
        term2 *= (seq_t)(2*k + 5);
        term2 *= (seq_t)(2*k + 7);
        term2 *= (seq_t)(k + 4);

        seq_t term3 = mem[k + 2];
        term3 *= (seq_t)8;
        term3 *= (seq_t)(2*k + 7);
        term3 *= (seq_t)(2*k + 5);
        term3 *= (seq_t)(k + 2);
        term3 *= (seq_t)(k + 4);
        term3 = -term3;

        seq_t sum = term1 + term2 + term3;
        mem[size] = -sum;

        ++size;
    }

    return mem[(size_t)n];
}
