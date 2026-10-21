/*
 * A210025  a(0)=1. Let S = string of digits of a(0),a(1),a(2),... To get
 *          a(n+1) (n>=0), subtract next element of S from a(n) if that would
 *          give a positive number, otherwise add it to a(n).
 * https://oeis.org/A210025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A210025:
   a(0) = 1.
   Let S be the decimal‐digit stream of the terms a(0),a(1),a(2),… .
   For n ≥ 0, let d(n) be the n‑th digit of S (0‑indexed).  Then
   a(n+1) = a(n) – d(n)  if  a(n) – d(n) > 0,
   otherwise        a(n+1) = a(n) + d(n). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static storage for the computed terms and the digit stream. */
static seq_t *terms      = NULL;   /* array of computed terms   */
static size_t terms_cap  = 0;      /* allocated capacity        */
static size_t terms_len  = 0;      /* number of terms stored    */

static uint8_t *digits   = NULL;   /* concatenated decimal digits of all terms */
static size_t   digits_cap = 0;    /* allocated capacity of digits buffer   */
static size_t   digits_len = 0;    /* number of digits currently stored     */
static size_t   digits_pos = 0;    /* index of the next digit to consume    */

/* Ensure the terms array has room for at least one more element. */
static void ensure_terms_capacity(void)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        terms_cap = new_cap;
    }
}

/* Ensure the digits buffer has room for at least one more digit. */
static void ensure_digits_capacity(void)
{
    if (digits_len == digits_cap) {
        size_t new_cap = digits_cap ? digits_cap * 2 : 32;
        uint8_t *tmp = (uint8_t *)realloc(digits, new_cap * sizeof(uint8_t));
        if (!tmp) abort();
        digits = tmp;
        digits_cap = new_cap;
    }
}

/* Append a single digit (0–9) to the digits buffer. */
static void push_digit(uint8_t d)
{
    ensure_digits_capacity();
    digits[digits_len++] = d;
}

/* Convert a seq_t value to its decimal digits and append them to the buffer. */
static void push_digits_of_value(seq_t value)
{
    /* Handle zero explicitly (never occurs in this sequence but keep it generic). */
    if (value == 0) {
        push_digit(0);
        return;
    }

    /* Extract digits in reverse order into a small temporary array. */
    uint8_t rev[64];               /* fits up to 2^512 (~10^150) comfortably */
    size_t rev_len = 0;
    seq_t tmp = value;
    while (tmp > 0) {
        rev[rev_len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    /* Append the digits in proper order to the global buffer. */
    for (size_t i = rev_len; i > 0; --i)
        push_digit(rev[i - 1]);
}

/* Append a new term to the terms array. */
static void push_term(seq_t term)
{
    ensure_terms_capacity();
    terms[terms_len++] = term;
}

/* Initialise static data with a(0)=1 and its digits. */
static void init_sequence(void)
{
    if (terms_len != 0) return;     /* already initialised */
    push_term((seq_t)1);             /* a(0) = 1 */
    push_digits_of_value((seq_t)1);  /* digits of a(0) */
    digits_pos = 0;                 /* first digit to consume */
}

/* Public function required by the exercise. */
seq_t a(idx_t n)
{
    /* Lazy initialisation. */
    init_sequence();

    /* Generate terms up to index n if necessary. */
    while (n >= terms_len) {
        /* Consume the next digit of S. */
        uint8_t d = digits[digits_pos++];
        seq_t prev = terms[terms_len - 1];
        seq_t next;

        if (prev > d)            /* remember d is small, implicit conversion to seq_t */
            next = prev - d;
        else
            next = prev + d;

        /* Store the new term and append its digits. */
        push_term(next);
        push_digits_of_value(next);
    }

    return terms[n];
}
