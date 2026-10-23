/*
 * A107859  Take the sequence bb with two integer "seeds", e.g. a(1)=0, a(2)=1
 *          and the relating list cc consisting of all digits of bb, e.g.
 *          cc={0,1}. Take n=3, calculate d=abs[cc(n-1)-cc(n-2)] = abs(2-3)=1;
 *          then next term of bb, a(n), is the smallest "new" (not in bb)
 *          number containing digit d, e.g. a(3)=10...
 * https://oeis.org/A107859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0, a(1) = 1. For n ≥ 2, let d = |cc[n‑1] – cc[n‑2]| where
 *            cc is the concatenated list of decimal digits of all previous
 *            terms (cc[1] is the first digit of a(0)). Then a(n) is the
 *            smallest positive integer not already in the sequence that
 *            contains the digit d. The sequence is memoised so th...
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   contains_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains_digit.h"

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t term_cap = 0, term_cnt = 0;
    static seq_t *used = NULL;
    static size_t used_cap = 0, used_cnt = 0;
    static uint8_t *cc = NULL;
    static size_t cc_cap = 0, cc_len = 0;

    /* initialisation */
    if (terms == NULL)
    {
        term_cap = 8;
        terms = (seq_t *)malloc(sizeof(seq_t) * term_cap);
        used = (seq_t *)malloc(sizeof(seq_t) * term_cap);
        cc_cap = 32;
        cc = (uint8_t *)malloc(cc_cap);

        /* seeds a(0)=0, a(1)=1 */
        terms[0] = (seq_t)0;
        terms[1] = (seq_t)1;
        term_cnt = 2;
        used[0] = (seq_t)0;
        used[1] = (seq_t)1;
        used_cnt = 2;

        /* digits of the seeds */
        cc[0] = 0;          /* cc[1] in 1‑based indexing */
        cc[1] = 1;          /* cc[2] */
        cc_len = 2;
    }

    /* grow arrays if necessary */
    while (term_cnt <= n)
    {
        if (term_cnt == term_cap)
        {
            term_cap *= 2;
            terms = (seq_t *)realloc(terms, sizeof(seq_t) * term_cap);
            used  = (seq_t *)realloc(used,  sizeof(seq_t) * term_cap);
        }

        /* compute d = |cc[term_cnt-1] – cc[term_cnt-2]|  (1‑based) */
        seq_t d0 = cc[term_cnt - 1];
        seq_t d1 = cc[term_cnt - 2];
        seq_t d  = (d0 > d1) ? (d0 - d1) : (d1 - d0);
        int digit = (int)d;                       /* guaranteed 0‑9 */

        /* search for the smallest unused number containing digit */
        seq_t cand = 0;
        while (true)
        {
            bool already = false;
            for (size_t i = 0; i < used_cnt; ++i)
            {
                if (used[i] == cand)
                {
                    already = true;
                    break;
                }
            }
            if (!already && contains_digit(cand, digit))
                break;
            ++cand;
        }

        /* store new term */
        terms[term_cnt] = cand;
        used[used_cnt]  = cand;
        ++term_cnt;
        ++used_cnt;

        /* append its decimal digits to cc */
        seq_t tmp = cand;
        if (tmp == 0)
        {
            if (cc_len == cc_cap) {
                cc_cap *= 2;
                cc = (uint8_t *)realloc(cc, cc_cap);
            }
            cc[cc_len++] = 0;
        }
        else
        {
            uint8_t buf[32];
            size_t len = 0;
            while (tmp > 0)
            {
                buf[len++] = (uint8_t)(tmp % 10);
                tmp = tmp / 10;
            }
            for (int i = (int)len - 1; i >= 0; --i)
            {
                if (cc_len == cc_cap) {
                    cc_cap *= 2;
                    cc = (uint8_t *)realloc(cc, cc_cap);
                }
                cc[cc_len++] = buf[i];
            }
        }
    }

    return terms[n];
}
