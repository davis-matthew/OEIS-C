/*
 * A102257  Begin with 3, multiply each digit by 2, separate the digits.
 * https://oeis.org/A102257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102257: start with 3.  For each term a(k) (k≥1),
   compute 2*a(k), write the decimal digits of the product in
   order as new terms of the sequence.  Each a(n) is the nth
   decimal digit produced in this process. */
/* Example: a(1)=3, a(2)=6; 6*2=12 → a(3)=1, a(4)=2; 1*2=2 → a(5)=2, etc. */

#include <stdlib.h>

static seq_t *seq = NULL;      /* array of computed terms */
static idx_t  len = 0;         /* number of terms stored */
static idx_t  cap = 0;         /* allocated capacity of seq */
static idx_t  next_process = 1;/* index (1‑based) of next term to process */

/* Grow the array so that it can hold at least new_len elements. */
static void ensure_capacity(idx_t new_len)
{
    if (cap >= new_len)
        return;
    idx_t new_cap = cap ? cap << 1 : 64;
    while (new_cap < new_len)
        new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    cap = new_cap;
}

/* Return the nth term of the sequence (n>=1). */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; behaviour for n=0 is irrelevant */
        return (seq_t)0;

    /* Initialisation on first call. */
    if (len == 0) {
        ensure_capacity(64);
        seq[0] = (seq_t)3;
        len = 1;
        next_process = 1;
    }

    /* Grow the sequence until we have at least n terms. */
    while (len < n) {
        seq_t val = seq[next_process - 1];
        seq_t prod = val * ((seq_t)2);

        /* Extract decimal digits of prod in reverse order. */
        idx_t digits[20];    /* 20 > max digits for 2*digit (18). */
        idx_t dsz = 0;
        seq_t tmp = prod;
        if (tmp == 0) {
            digits[dsz++] = 0;
        } else {
            while (tmp != (seq_t)0) {
                digits[dsz++] = (idx_t)(tmp % (seq_t)10);
                tmp /= (seq_t)10;
            }
        }

        /* Append all digits to the sequence in correct order. */
        for (idx_t j = dsz; j > 0; --j) {
            ensure_capacity(len + 1);
            seq[len++] = (seq_t)digits[j - 1];
        }

        /* Prepare to process the next term. */
        ++next_process;
    }

    return seq[n - 1];
}
