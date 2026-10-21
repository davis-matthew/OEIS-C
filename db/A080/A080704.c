/*
 * A080704  a(1)=2; for n>1, if n is in the sequence then a(n) is the smallest
 *          even integer > a(n-1), otherwise a(n) = a(n-1) + 3.
 * https://oeis.org/A080704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A080704
   Definition: a(1)=2.
                For n>1, if n is itself a term of the sequence then
                a(n)=smallest even integer > a(n-1),
                otherwise a(n)=a(n-1)+3. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *terms = NULL;      /* array of computed a(i) */
    static size_t terms_cap = 0;     /* allocated capacity (in items) */
    static idx_t terms_len = 0;      /* number of terms stored */

    static unsigned char *used = NULL;       /* bitset of values already seen */
    static size_t used_cap = 0;               /* allocated capacity (in bytes) */

    /* expand terms array to hold up to n terms */
    if (terms_len < n) {
        size_t need = (size_t)n;
        if (terms_cap < need) {
            size_t newcap = terms_cap ? terms_cap : 16;
            while (newcap < need) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
            if (!tmp) abort();          /* allocation failure */
            terms = tmp;
            terms_cap = newcap;
        }

        /* allocate used array if first time */
        if (!used) {
            used_cap = 1024;
            unsigned char *tmp = (unsigned char *)calloc(used_cap, 1);
            if (!tmp) abort();
            used = tmp;
        }

        /* compute terms[i] for i = terms_len+1 .. n */
        for (idx_t i = terms_len + 1; i <= n; ++i) {
            seq_t next_val;
            if (i == 1) {
                next_val = 2;                       /* a(1) = 2 */
            } else {
                /* membership test: is i already a value of the sequence? */
                bool is_in = false;
                if ((size_t)i < used_cap) is_in = used[(size_t)i];
                if (is_in) {
                    seq_t prev = terms[terms_len - 1];
                    /* smallest even integer > prev */
                    if ((prev & 1) == 0) next_val = prev + 2;
                    else                next_val = prev + 1;   /* prev odd → next even */
                } else {
                    next_val = terms[terms_len - 1] + 3;
                }
            }

            /* store the new term */
            terms[terms_len++] = next_val;

            /* mark this value as seen for future membership tests */
            size_t idx = (size_t)next_val;
            if (idx >= used_cap) {
                size_t newcap = used_cap ? used_cap : 1;
                while (newcap <= idx) newcap <<= 1;
                unsigned char *tmp = (unsigned char *)realloc(used, newcap);
                if (!tmp) abort();
                memset(tmp + used_cap, 0, newcap - used_cap);
                used = tmp;
                used_cap = newcap;
            }
            used[idx] = 1;
        }
    }

    return terms[n - 1];
}
