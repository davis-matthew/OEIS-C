/*
 * A103697  Add 5 to each of the preceding digits, beginning with 0.
 * https://oeis.org/A103697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(0)=0.  For n≥0, the next digit d is taken from a stream
   (initially containing 0).  a(n)=d+5.  The decimal digits of a(n)
   are appended to the end of the stream.  This implements OEIS A103697. */

#include <stddef.h>
#include <stdint.h>

#define MAX_TERMS 200000      /* sufficient for n up to ~10 000 and beyond */
#define MAX_DIGITS 400000     /* sufficient as each term adds at most 2 digits */

static seq_t terms[MAX_TERMS];
static size_t terms_len = 0;   /* number of terms computed */

/* Digits stream */
static uint8_t digits[MAX_DIGITS];
static size_t digits_len = 0;  /* total digits stored */
static size_t digits_next = 0; /* index of next digit to consume */

seq_t a(idx_t n)
{
    /* Initialise the structure on first call */
    if (terms_len == 0) {
        terms[0] = (seq_t)0;
        terms_len = 1;

        digits[0] = 0;
        digits_len = 1;
        digits_next = 0;
    }

    /* Return already computed value */
    if ((size_t)n < terms_len)
        return terms[n];

    /* Generate terms until the desired index is reached */
    while (terms_len <= (size_t)n) {
        /* Safe guard – the stream always contains a next digit */
        if (digits_next >= digits_len)
            break;   /* under normal conditions this never happens */

        uint8_t d = digits[digits_next++];
        seq_t term = (seq_t)d + (seq_t)5; /* term is between 5 and 14 */

        /* Store the term */
        if (terms_len >= MAX_TERMS)
            break;   /* overflow guard – enough capacity for test cases */
        terms[terms_len++] = term;

        /* Append decimal digits of the term to the stream */
        unsigned long long t = (unsigned long long)term; /* safe conversion */
        if (t >= 10) {
            /* Two‑digit number (10–14) */
            if (digits_len + 2 >= MAX_DIGITS)
                break; /* guard against overflow */
            digits[digits_len++] = (uint8_t)(t / 10);
            digits[digits_len++] = (uint8_t)(t % 10);
        } else {
            /* One‑digit number (5–9) */
            if (digits_len + 1 >= MAX_DIGITS)
                break; /* guard against overflow */
            digits[digits_len++] = (uint8_t)t;
        }
    }

    /* If we reached here and still did not compute a(n), the requested
       index exceeds our preallocated capacity.  In that extremely unlikely
       case, return 0 as a safe fallback. */
    if ((size_t)n >= terms_len)
        return (seq_t)0;

    return terms[n];
}
