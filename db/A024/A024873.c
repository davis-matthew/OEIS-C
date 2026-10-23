/*
 * A024873  a(n) = s(1)*t(n) + s(2)*t(n-1) + ... + s(k)*t(n-k+1), where k = [
 *          n/2 ], s = (natural numbers >= 2), t = (Lucas numbers).
 * https://oeis.org/A024873
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) =
 *            a(n-1)+3*a(n-2)-2*a(n-3)-a(n-4)-a(n-5)-3*a(n-6)+2*a(n-7)+a(n-8)+a(n-9)+a(n-10);
 *            Initial values: a(2)=6, a(3)=8, a(4)=26, a(5)=43, a(6)=97,
 *            a(7)=156, a(8)=308, a(9)=499, a(10)=915, a(11)=1480
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq = NULL;          /* cache of a(n) values */
static size_t seq_cap = 0;         /* allocated capacity (in elements) */
static idx_t  seq_max = 1;         /* highest n that has been computed */

/* Ensure that seq[n] is defined for all n <= limit, computing values as needed */
static void ensure(idx_t limit)
{
    if (limit < 2) return;

    /* First allocation: initialise basic terms up to a(11) */
    if (!seq) {
        size_t cap = 12;                      /* indices 0..11 */
        while (cap <= (size_t)limit) cap <<= 1;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!seq) abort();

        seq_cap = cap;
        seq[2]  = (seq_t)6;
        seq[3]  = (seq_t)8;
        seq[4]  = (seq_t)26;
        seq[5]  = (seq_t)43;
        seq[6]  = (seq_t)97;
        seq[7]  = (seq_t)156;
        seq[8]  = (seq_t)308;
        seq[9]  = (seq_t)499;
        seq[10] = (seq_t)915;
        seq[11] = (seq_t)1480;
        seq_max = 11;
    }

    if (limit <= seq_max) return;

    /* Grow the array if necessary */
    if ((size_t)limit + 1 > seq_cap) {
        size_t cap = seq_cap;
        while ((size_t)limit + 1 > cap) cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, cap * sizeof(seq_t));
        if (!tmp) abort();
        seq = tmp;
        seq_cap = cap;
    }

    /* Compute successive terms using the recurrence */
    for (idx_t n = seq_max + 1; n <= limit; ++n) {
        if (n <= 11) continue;            /* already set */

        /* Positive contributions */
        seq_t pos = seq[n-1];
        /* 3*a(n-2) */
        pos += seq[n-2];
        pos += seq[n-2];
        pos += seq[n-2];
        /* 2*a(n-7) */
        pos += seq[n-7];
        pos += seq[n-7];
        /* a(n-8), a(n-9), a(n-10) */
        pos += seq[n-8];
        pos += seq[n-9];
        pos += seq[n-10];

        /* Negative contributions */
        seq_t neg = seq[n-3];
        neg += seq[n-3];          /* 2*a(n-3) */
        neg += seq[n-4];          /* a(n-4) */
        neg += seq[n-5];          /* a(n-5) */
        /* 3*a(n-6) */
        neg += seq[n-6];
        neg += seq[n-6];
        neg += seq[n-6];

        seq[n] = pos - neg;       /* pos > neg for all n >= 12 */
    }
    seq_max = limit;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* out of valid range (not used) */
    ensure(n);
    return seq[n];
}
