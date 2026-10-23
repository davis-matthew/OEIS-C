/*
 * A132671  a(1)=1, a(n) = 7*a(n-1) if the minimal positive integer not yet in
 *          the sequence is greater than a(n-1), else a(n) = a(n-1) - 1.
 * https://oeis.org/A132671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n) = a(n-1)-1 if a(n-1)-1>0 and not yet in the
 *            sequence, otherwise a(n) = 7*a(n-1)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below memoises the sequence and checks membership by
   linear search, which is sufficient for the required n up to about
   10 000. All allocations are resized with a power‑of‑two policy. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *seq = NULL;          /* array of computed terms */
    static size_t seq_cap = 0;          /* capacity of seq array */
    static idx_t seq_len = 0;           /* number of terms computed so far */

    static seq_t *used = NULL;         /* array of all used values */
    static size_t used_cap = 0;         /* capacity of used array */
    static size_t used_len = 0;         /* number of used values */

    /* initialisation on first call */
    if (seq_len == 0) {
        /* allocate space for terms (start small, double as needed) */
        seq_cap = 64;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq) abort();
        seq[0] = (seq_t)1;            /* a(1) = 1 */
        seq_len = 1;

        /* allocate space for used values (start small, double as needed) */
        used_cap = 128;
        used = (seq_t*)malloc(used_cap * sizeof(seq_t));
        if (!used) abort();
        used[0] = (seq_t)1;           /* value 1 is used */
        used_len = 1;
    }

    /* generate terms up to n (inclusive) if necessary */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        seq_t prev = seq[seq_len - 1];
        seq_t dec = prev - (seq_t)1;
        seq_t next;

        /* linear search to see if dec has been used */
        int found = 0;
        if (dec > (seq_t)0) {
            for (size_t k = 0; k < used_len; ++k) {
                if (used[k] == dec) {
                    found = 1;
                    break;
                }
            }
        }

        if (dec > (seq_t)0 && !found) {
            next = dec;
        } else {
            next = prev * (seq_t)7;
        }

        /* store next in the used array (resize if needed) */
        if (used_len == used_cap) {
            size_t new_cap = used_cap << 1;
            seq_t *tmp = (seq_t*)realloc(used, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            used = tmp;
            used_cap = new_cap;
        }
        used[used_len++] = next;

        /* store next in the seq array (resize if needed) */
        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap << 1;
            seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            seq = tmp;
            seq_cap = new_cap;
        }
        seq[seq_len++] = next;
    }

    return seq[n - 1];
}
