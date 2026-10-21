/*
 * A233580  In balanced ternary notation, zerofree non-repdigit numbers that
 *          are either palindromes or sign reversed palindromes.
 * https://oeis.org/A233580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = enumerates balanced ternary zero‑free numbers that are *
 *            palindromes or sign‑reversed palindromes, in increasing order
 * Range:     u8 n <= 8, u16 n <= 98, u32 n <= 3078, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Comparator for qsort over seq_t values. */
static int cmp_seq_t(const void *pa, const void *pb)
{
    seq_t a = *(const seq_t *)pa;
    seq_t b = *(const seq_t *)pb;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Temporary buffer large enough for the maximum number of candidates
     * generated in a single length (up to length 30 for n≈10000). */
    static seq_t buf[32768];
    idx_t count = 0;        /* How many terms have been collected */

    for (idx_t L = 1; ; ++L)   /* `L` = length in balanced ternary digits   */
    {
        /* ---------- Palindromes (including odd lengths) ---------- */
        idx_t k_pal = (L + 1) / 2;                 /* ceil(L/2)          */
        idx_t mask_start_pal = ((idx_t)1) << (k_pal - 1); /* leading digit = +1*/
        idx_t mask_end_pal   = ((idx_t)1) << k_pal;       /* exclusive */

        size_t pos = 0;      /* position in buf */

        for (idx_t mask = mask_start_pal; mask < mask_end_pal; ++mask)
        {
            /* Skip the repdigit case (all digits +1) */
            if (mask == mask_end_pal - 1)
                continue;

            int digits[64];
            /* First half (including middle if L odd) */
            for (int i = 0; i < (int)k_pal; ++i)
            {
                int bit = (mask >> (k_pal - 1 - i)) & 1U;
                digits[i] = bit ? 1 : -1;
            }
            /* Mirror to complete the palindrome */
            for (int i = (int)k_pal; i < (int)L; ++i)
            {
                digits[i] = digits[L - 1 - i];
            }
            /* Convert to decimal value */
            seq_t v = 0;
            for (int i = 0; i < (int)L; ++i)
            {
                v = v * 3;
                v += (digits[i] == 1) ? 1 : -1;
            }
            buf[pos++] = v;
        }

        /* ---------- Sign‑reversed palindromes (even lengths only) ---------- */
        if ((L & 1) == 0)          /* L is even */
        {
            idx_t k_sr = L / 2;                    /* half length       */
            idx_t mask_start_sr = ((idx_t)1) << (k_sr - 1);
            idx_t mask_end_sr   = ((idx_t)1) << k_sr;

            for (idx_t mask = mask_start_sr; mask < mask_end_sr; ++mask)
            {
                int digits[64];
                /* First half */
                for (int i = 0; i < (int)k_sr; ++i)
                {
                    int bit = (mask >> (k_sr - 1 - i)) & 1U;
                    digits[i] = bit ? 1 : -1;
                }
                /* Second half is negative of the reflected first half */
                for (int i = k_sr; i < (int)L; ++i)
                {
                    digits[i] = -digits[L - 1 - i];
                }
                /* Convert to decimal value */
                seq_t v = 0;
                for (int i = 0; i < (int)L; ++i)
                {
                    v = v * 3;
                    v += (digits[i] == 1) ? 1 : -1;
                }
                buf[pos++] = v;
            }
        }

        /* Sort the candidates for the current length. They are not necessarily
         * already sorted because we merged two types of fragments. */
        qsort(buf, pos, sizeof(seq_t), cmp_seq_t);

        /* Scan through the sorted list and return when we reach the nth term. */
        for (size_t i = 0; i < pos; ++i)
        {
            if (count + 1 == n)
                return buf[i];
            ++count;
        }
        /* If we haven't returned yet, go to the next length. */
    }
}
