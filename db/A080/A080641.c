/*
 * A080641  a(1) = 4; for n>1, a(n) is taken to be the smallest integer
 *          greater than a(n-1) which is consistent with the condition "n is a
 *          member of the sequence if and only if a(n) is divisible by 5".
 * https://oeis.org/A080641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 4. For n>1, a(n) is the smallest integer larger than
 *            a(n-1) that satisfies the self‑referential rule: n is a member
 *            of the sequence (i.e., n occurs as some a(k)) iff a(n) is
 *            divisible by 5
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t  a_cap = 0;      /* capacity of a_values array            */
static idx_t  a_size = 0;     /* largest index computed so far         */
static seq_t *a_values = NULL;/* 1‑based array of sequence values      */

static idx_t  m_cap = 0;      /* capacity of membership boolean array  */
static uint8_t *is_member = NULL;/* 1‑based boolean array                */

/* Grow the a_values array to at least 'min_cap' elements.            */
static void ensure_a_cap(idx_t min_cap)
{
    if (min_cap <= a_cap) return;
    idx_t new_cap = a_cap ? a_cap : 1;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_values, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_values = tmp;
    a_cap   = new_cap;
}

/* Grow the is_member array to at least 'min_cap' elements.            */
static void ensure_m_cap(idx_t min_cap)
{
    if (min_cap <= m_cap) return;
    idx_t new_cap = m_cap ? m_cap : 1;
    while (new_cap < min_cap) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(is_member, new_cap * sizeof(uint8_t));
    if (!tmp) abort();
    /* initialise newly added entries to 0 */
    for (idx_t i = m_cap; i < new_cap; ++i) tmp[i] = 0;
    is_member = tmp;
    m_cap     = new_cap;
}

/* Initialise static arrays on first entry.                           */
static void init_sequence(void)
{
    if (a_values) return;           /* already initialised */
    /* allocate initial arrays */
    a_cap = 16;                     /* enough for early indices           */
    a_values = (seq_t *)calloc(a_cap, sizeof(seq_t));
    if (!a_values) abort();

    m_cap = 128;                    /* accommodate early values            */
    is_member = (uint8_t *)calloc(m_cap, sizeof(uint8_t));
    if (!is_member) abort();

    /* base case: a(1) = 4 */
    a_values[1] = (seq_t)4;
    a_size = 1;

    /* mark that value 4 occurs in the sequence */
    ensure_m_cap((idx_t)4 + 1);
    is_member[4] = 1;
}

/* Return the nth member of the sequence.                               */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* out of range – not used          */
    init_sequence();

    /* ensure we have storage for indices up to n and values seen so far */
    if (n > a_size) {
        /* expand a_values array to accommodate new indices */
        ensure_a_cap(n + 1);
        /* expand is_member array to cover indices up to n */
        ensure_m_cap(n + 1);

        for (idx_t k = a_size + 1; k <= n; ++k) {
            /* decide whether a(k) must be divisible by 5 */
            bool need_div = is_member[k] ? true : false;

            seq_t prev = a_values[k - 1];
            seq_t cand = prev + (seq_t)1;

            if (need_div) {
                seq_t r = cand % (seq_t)5;
                if (r != (seq_t)0) cand += (seq_t)(5 - r);
            } else {
                if (cand % (seq_t)5 == (seq_t)0) cand += (seq_t)1;
            }

            a_values[k] = cand;

            /* this value now occurs in the sequence, so mark it   */
            idx_t val_idx = (idx_t)cand;
            if (val_idx > m_cap) ensure_m_cap(val_idx + 1);
            is_member[val_idx] = 1;
        }
        a_size = n;
    }

    return a_values[n];
}
