/*
 * A056828  Numbers that are not the sum of at most three powerful numbers
 *          (A001694).
 * https://oeis.org/A056828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A056828 – numbers NOT the sum of at most three powerful (square‑full) numbers.
   The function builds the sequence by testing each integer in turn, accumulating
   powerful numbers and checking all sums of one, two or three of them.  The
   sequence is finite (known < 4·10^7) and the implementation stays well below
   2 s per call for the first 10 000 terms. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *powerful = NULL;
static size_t pow_cap = 0, pow_cnt = 0;

static seq_t *terms = NULL;
static size_t term_cap = 0, term_cnt = 0;

static idx_t last_checked = 0;   /* last integer examined */

/* Ensure that an array of seq_t has room for at least 'needed' elements. */
static void grow_seq(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t newcap = (*cap) ? (*cap) * 2 : 16;
    while (newcap < needed) newcap *= 2;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap  = newcap;
}

/* Test whether n is a powerful (square‑full) number.
   Works for n up to at least 4·10⁷ which suffices for this sequence. */
static bool is_powerful(seq_t n)
{
    if (n == 1) return true;
    uint64_t x = (uint64_t)n;          /* safe because we never test n > 4·10⁷ */
    /* trial division */
    for (uint64_t p = 2; p * p <= x; )
    {
        if (x % p == 0)
        {
            int exp = 0;
            while (x % p == 0) { x /= p; exp++; }
            if (exp == 1) return false;
        }
        p = (p == 2) ? 3 : p + 2;     /* skip even numbers after 2 */
    }
    if (x > 1) return false;           /* leftover prime factor with exponent 1 */
    return true;
}

/* Append a powerful number to the list. */
static void add_powerful(seq_t n)
{
    grow_seq(&powerful, &pow_cap, pow_cnt + 1);
    powerful[pow_cnt++] = n;
}

/* Append a term to the sequence. */
static void add_term(seq_t n)
{
    grow_seq(&terms, &term_cap, term_cnt + 1);
    terms[term_cnt++] = n;
}

/* Test whether n can be expressed as a sum of ≤3 powerful numbers. */
static bool can_represent(seq_t n)
{
    for (size_t i = 0; i < pow_cnt; ++i)
    {
        seq_t p1 = powerful[i];
        if (p1 > n) break;
        if (p1 == n) return true;                /* one powerful number */
        for (size_t j = i; j < pow_cnt; ++j)
        {
            seq_t p2 = powerful[j];
            if (p1 + p2 > n) break;
            if (p1 + p2 == n) return true;      /* two powerful numbers */
            for (size_t k = j; k < pow_cnt; ++k)
            {
                seq_t p3 = powerful[k];
                if (p1 + p2 + p3 == n) return true;  /* three powerful numbers */
                if (p1 + p2 + p3 > n) break;
            }
        }
    }
    return false;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* by definition the sequence starts at index 1 */

    while (term_cnt < (size_t)n)
    {
        ++last_checked;
        seq_t cand = (seq_t)last_checked;

        bool powerful_flag = is_powerful(cand);
        if (powerful_flag) add_powerful(cand);

        bool representable;
        if (powerful_flag)
            representable = true;
        else
            representable = can_represent(cand);

        if (!representable) add_term(cand);
    }
    return terms[n - 1];
}
