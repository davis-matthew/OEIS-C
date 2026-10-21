/*
 * A226245  Locations of successive record-setters (maxima) in Hofstadter's
 *          Q-sequence.
 * https://oeis.org/A226245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 564, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) is the first index where the nth record‑setter in
   Hofstadter's Q‑sequence appears.  The function generates the Q‑sequence
   lazily, keeps track of new record maxima and returns the position
   corresponding to the requested rank n.  */ 

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; return 0 for safety    */
        return (seq_t)0;

    /* ----- static state kept between calls -------------------------------- */
    static seq_t *q      = NULL;   /* Q‑sequence, 1‑based, q[0] unused   */
    static size_t q_cap  = 0;      /* largest index that can be stored    */
    static idx_t  q_len  = 0;      /* last index computed in q            */
    static seq_t current_max = 1;  /* current record maximum              */

    static seq_t *rec     = NULL;   /* record positions, 0‑based          */
    static size_t rec_cap  = 0;     /* allocated length of rec array      */
    static size_t rec_len  = 0;     /* number of record positions stored  */

    /* ----- first‑time initialization ------------------------------------- */
    if (!q) {
        q_cap = 4;                                   /* initial capacity */
        q = (seq_t*)malloc(sizeof(seq_t)*(q_cap+1));
        q[1] = q[2] = 1;
        q_len = 2;

        rec_cap = 16;                                 /* room for many records  */
        rec = (seq_t*)malloc(sizeof(seq_t)*rec_cap);
        rec[0] = 1;                                   /* a(1) = 1 */
        rec_len = 1;
    }

    /* ----- generate Q(n) until we have found the nth record position ---- */
    while (rec_len < (size_t)n) {
        idx_t m = q_len + 1;                         /* next index to compute */

        /* Q(m) = Q(m - Q(m-1)) + Q(m - Q(m-2)) */
        idx_t idx1 = m - (idx_t)q[m-1];
        idx_t idx2 = m - (idx_t)q[m-2];
        seq_t val  = q[(size_t)idx1] + q[(size_t)idx2];

        /* grow q array if necessary (1‑based, so capacity >= m) */
        if ((size_t)m > q_cap) {
            size_t new_cap = q_cap * 2;
            while (new_cap < (size_t)m) new_cap *= 2;
            q = (seq_t*)realloc(q, (new_cap+1)*sizeof(seq_t));
            q_cap = new_cap;
        }
        q[m] = val;
        q_len = m;

        /* record new maximum if it occurs */
        if (val > current_max) {
            current_max = val;

            /* grow record array if necessary */
            if (rec_len >= rec_cap) {
                size_t new_cap = rec_cap * 2 + 1;
                rec = (seq_t*)realloc(rec, new_cap*sizeof(seq_t));
                rec_cap = new_cap;
            }
            rec[rec_len] = m;     /* record position for this new record */
            rec_len++;
        }
    }

    return rec[n-1];     /* a(n) is stored at rec[n-1] */
}
