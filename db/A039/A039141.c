/*
 * A039141  Numbers whose base-10 representation has the same number of 4's
 *          and 5's.
 * https://oeis.org/A039141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description: a(n) returns the nth integer whose base‑10 representation
   contains an equal number of digits 4 and 5 (OEIS A039141).  The function
   builds the sequence on demand and memoizes all terms that have been
   computed, so calls may come in any order. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_storage = NULL;   /* cached terms */
static size_t   seq_size   = 0;     /* number of terms stored */
static size_t   seq_cap    = 0;     /* allocated capacity */
static idx_t    candidate  = 0;     /* next integer to test */

/* Ensure that the storage can hold at least `need` elements.  */
static void
ensure_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_storage, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();   /* out of memory */
    seq_storage = tmp;
    seq_cap = new_cap;
}

/* Return the nth term (n >= 1) of the sequence.  */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for offset 0, harmless default */

    /* Generate terms until we have at least n of them. */
    while (seq_size < n) {
        seq_t val = (seq_t)candidate;

        /* Count digits 4 and 5.  */
        unsigned count4 = 0, count5 = 0;
        if (val != 0) {
            seq_t tmp = val;
            while (tmp > 0) {
                unsigned d = (unsigned)(tmp % 10);
                if (d == 4)
                    ++count4;
                else if (d == 5)
                    ++count5;
                tmp = tmp / 10;
            }
        }

        /* Accept numbers where the counts are equal.  */
        if (count4 == count5) {
            ensure_capacity(seq_size + 1);
            seq_storage[seq_size++] = val;
        }

        ++candidate;
    }

    return seq_storage[n - 1];
}
