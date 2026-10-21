/*
 * A080652  a(1)=2; for n>1, a(n)=a(n-1)+3 if n is already in the sequence,
 *          a(n)=a(n-1)+2 otherwise.
 * https://oeis.org/A080652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2; for n>1, a(n)=a(n‑1)+3 if n is already in the sequence,
   otherwise a(n)=a(n‑1)+2. */

#include <stdlib.h>
#include <stdbool.h>

/* The following function stores computed terms and a Boolean array that
   records whether a given integer has already appeared as a sequence value.
   Both structures grow on demand so the function works for successive,
   out‑of‑order calls.  The implementation is valid C11 and C++17. */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *seq      = NULL;   /* array of computed terms      */
    static idx_t seq_len   = 0;       /* number of terms stored       */
    static size_t seq_cap  = 0;       /* allocation capacity for seq  */

    static bool *in_seq    = NULL;   /* table: in_seq[i] == true iff i
                                        has appeared as a sequence value  */
    static idx_t in_cap    = 0;       /* maximum index that in_seq
                                        currently covers                */

    /* Offset is 1; any call with n==0 is undefined behaviour,
       but we return 0 for safety. */
    if (n == 0) return 0;

    /* Initialise the sequence once: a(1)=2 */
    if (seq_len == 0) {
        /* allocate capacity for at least 128 terms */
        { size_t new_cap = 128; seq_cap = new_cap; }
        seq = (seq_t*)realloc(seq, seq_cap * sizeof(*seq));
        /* allocate Boolean table up to at least 2 (the first value) */
        if (in_cap < 2) {
            idx_t new_cap = 128;
            if (new_cap < 2) new_cap = 2;
            in_seq = (bool*)realloc(in_seq, (new_cap + 1) * sizeof(*in_seq));
            for (idx_t i = in_cap + 1; i <= new_cap; ++i) in_seq[i] = false;
            in_cap = new_cap;
        }
        seq[0] = (seq_t)2;
        seq_len = 1;
        in_seq[2] = true;         /* value 2 has appeared */
    }

    /* If the desired index is already computed, return it */
    if (n <= seq_len) return seq[n - 1];

    /* Grow the terms array if necessary */
    if ((size_t)n > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap : 1;
        while ((size_t)n > new_cap) new_cap <<= 1;
        seq = (seq_t*)realloc(seq, new_cap * sizeof(*seq));
        seq_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t k = seq_len + 1; k <= n; ++k) {
        seq_t prev = seq[seq_len - 1];

        /* Did the index k already appear as a sequence value? */
        bool k_in = (k <= in_cap) ? in_seq[k] : false;

        seq_t new_val = prev + (k_in ? (seq_t)3 : (seq_t)2);

        /* Ensure the Boolean table covers the new value, if within idx_t. */
        if (new_val <= (seq_t)in_cap) {
            /* safe: already covered */
        } else {
            /* new_val > in_cap: grow the table.  We cast in_cap to seq_t
               for a comparison that works for arbitrary integer types. */
            if (new_val > (seq_t)in_cap) {
                idx_t needed = (idx_t)new_val;
                idx_t new_cap = in_cap ? in_cap : 1;
                while (needed > new_cap) new_cap <<= 1;
                in_seq = (bool*)realloc(in_seq, (new_cap + 1) * sizeof(*in_seq));
                for (idx_t i = in_cap + 1; i <= new_cap; ++i) in_seq[i] = false;
                in_cap = new_cap;
            }
        }

        /* Mark the value as seen *after* we grew the table if necessary. */
        if (new_val <= (seq_t)in_cap) {
            in_seq[(idx_t)new_val] = true;
        }

        seq[seq_len] = new_val;
        seq_len++;
    }

    return seq[n - 1];
}
