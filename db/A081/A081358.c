/*
 * A081358  E.g.f.: log((1+x) / (1-x)) / (2*(1-x)).
 * https://oeis.org/A081358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + ((-1)^(n+1)+1)/2*(n-1)! (a(0)=0)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static storage for factorials and a(n) values.
       They are expanded lazily on first use and on demand. */
    static seq_t *fact = NULL;
    static seq_t *a_arr = NULL;
    static size_t cap = 0;     /* current capacity of arrays */
    static size_t last = 0;    /* last index that has been filled */

    /* Expand arrays up to index n if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap << 1 : 1;      /* double until big enough */
        while (new_cap <= (size_t)n) new_cap <<= 1;

        if (fact == NULL) {                       /* first allocation */
            fact  = (seq_t*)calloc(new_cap, sizeof(seq_t));
            a_arr = (seq_t*)calloc(new_cap, sizeof(seq_t));
            /* compute from 0 up to new_cap-1 */
            for (size_t i = 0; i < new_cap; ++i) {
                if (i == 0) {
                    fact[i] = (seq_t)1;
                    a_arr[i] = (seq_t)0;
                } else {
                    fact[i] = fact[i-1] * (seq_t)i;
                    if (i & 1ULL) { /* odd i */
                        a_arr[i] = (seq_t)i * a_arr[i-1] + fact[i-1];
                    } else {         /* even i */
                        a_arr[i] = (seq_t)i * a_arr[i-1];
                    }
                }
            }
            last = new_cap - 1;
            cap  = new_cap;
        } else {                                   /* enlarge existing arrays */
            fact  = (seq_t*)realloc(fact,  new_cap * sizeof(seq_t));
            a_arr = (seq_t*)realloc(a_arr,  new_cap * sizeof(seq_t));
            /* compute only the newly required entries */
            for (size_t i = last + 1; i < new_cap; ++i) {
                fact[i] = fact[i-1] * (seq_t)i;
                if (i & 1ULL) { /* odd i */
                    a_arr[i] = (seq_t)i * a_arr[i-1] + fact[i-1];
                } else {         /* even i */
                    a_arr[i] = (seq_t)i * a_arr[i-1];
                }
            }
            last = new_cap - 1;
            cap  = new_cap;
        }
    }

    return a_arr[(size_t)n];
}
