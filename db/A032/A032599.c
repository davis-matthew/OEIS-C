/*
 * A032599  Cubes of lucky numbers.
 * https://oeis.org/A032599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000959(n)^3
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Static storage for precomputed lucky numbers */
static seq_t *lucky = NULL;
static idx_t lucky_cap = 0;

/* Ensure that at least 'needed' lucky numbers are precomputed. */
static void ensure_lucky(idx_t needed)
{
    if (lucky_cap >= needed)
        return;

    /* Start with a generous upper bound on the value of the needed lucky
       number.  The factor 25 works for n up to at least 10 000. */
    idx_t limit = needed * 25 + 100;

    while (1) {
        /* Only odd numbers are needed after the first sieve step. */
        idx_t init_size = limit / 2 + 10;
        seq_t *a = (seq_t *)malloc(sizeof(seq_t) * init_size);
        seq_t *b = (seq_t *)malloc(sizeof(seq_t) * init_size);
        if (!a || !b) {
            /* Allocation failed – abort; the sequence is not used in this
               environment */
            exit(EXIT_FAILURE);
        }

        /* Initialise the list with all odd numbers up to 'limit' */
        idx_t sz = 0;
        for (idx_t v = 1; v <= limit; v += 2)
            a[sz++] = v;

        /* Perform the lucky sieve on the odd list.  Each iteration removes
           every 'step'-th element from the current list. */
        idx_t pos = 1;                 /* start with the second element */
        while (pos < sz) {
            seq_t step = a[pos];
            if (step > sz) /* no more removals will happen */
                break;

            idx_t new_sz = 0;
            for (idx_t j = 0; j < sz; ++j) {
                /* 1‑based position of the element in the current list */
                if ((j + 1) % step != 0) {
                    b[new_sz++] = a[j];
                }
            }
            /* Prepare for next iteration */
            sz = new_sz;
            seq_t *tmp = a;
            a = b;
            b = tmp;
            ++pos;
        }

        /* If we have produced enough lucky numbers, store them. */
        if (sz >= needed) {
            if (lucky) free(lucky);
            lucky = (seq_t *)malloc(sizeof(seq_t) * sz);
            memcpy(lucky, a, sizeof(seq_t) * sz);
            lucky_cap = sz;
            free(a);
            free(b);
            break;
        }

        /* Not enough numbers – double the search limit and try again. */
        free(a);
        free(b);
        limit *= 2;
    }
}

/* Sequence function: a(n) = (A000959(n))^3 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset is 1; calls with n==0 are undefined */
    ensure_lucky(n);
    seq_t x = lucky[n - 1];
    return x * x * x;
}
