/*
 * A072199  a(n)-th factorial is the smallest factorial containing exactly n
 *          5's, or 0 if no such number exists.
 * https://oeis.org/A072199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = smallest m such that the decimal representation of m! contains exactly n digits equal to 5;
   return 0 if no such m exists. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* the sequence is defined only for n ≥ 1 */
    if (n < 1)
        return 0;

    /* ---------- static state --------------------------------------- */
    static bool init = false;
    static unsigned int *fac = NULL;        /* factorial digits in base 10000 (little endian) */
    static size_t fac_len = 0, fac_cap = 0;
    static seq_t *ans = NULL;               /* ans[k] = smallest m giving exactly k 5‑digits in m! (0 if unknown) */
    static size_t ans_cap = 0;
    static idx_t cur_m = 0;                 /* factorial currently stored is cur_m! */

    /* table of number of decimal 5’s in a 4‑digit block 0…9999 */
    static uint8_t digit5[10000];

    /* ---------- initialise static data -------------------------------- */
    if (!init) {
        for (unsigned int v = 0; v < 10000; ++v) {
            unsigned int x = v, cnt = 0;
            for (int i = 0; i < 4; ++i) {
                if ((x % 10) == 5)
                    ++cnt;
                x /= 10;
            }
            digit5[v] = (uint8_t)cnt;
        }
        fac_cap = 32;
        fac = (unsigned int*)malloc(fac_cap * sizeof(unsigned int));
        fac_len = 1;
        fac[0] = 1;          /* 0! = 1 */
        cur_m = 0;

        ans_cap = 32;
        ans = (seq_t*)calloc(ans_cap, sizeof(seq_t));

        init = true;
    }

    /* enlarge answer array if the requested n is beyond current capacity */
    if ((size_t)n >= ans_cap) {
        size_t old = ans_cap;
        size_t new_cap = ans_cap ? ans_cap << 1 : 32;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        ans = (seq_t*)realloc(ans, new_cap * sizeof(seq_t));
        memset(ans + old, 0, (new_cap - old) * sizeof(seq_t));
        ans_cap = new_cap;
    }

    /* if we already know the answer, return it */
    if (ans[n] != 0)
        return ans[n];

    /* ---------- iterate factorials until we see n 5‑digits ---------------- */
    while (1) {
        /* multiply current factorial by (cur_m + 1) */
        idx_t next = cur_m + 1;
        unsigned int carry = 0;
        for (size_t i = 0; i < fac_len; ++i) {
            unsigned long long prod = (unsigned long long)fac[i] * next + carry;
            fac[i] = (unsigned int)(prod % 10000);
            carry = (unsigned int)(prod / 10000);
        }
        while (carry) {
            if (fac_len == fac_cap) {
                fac_cap <<= 1;
                fac = (unsigned int*)realloc(fac, fac_cap * sizeof(unsigned int));
            }
            fac[fac_len++] = carry % 10000;
            carry /= 10000;
        }
        ++cur_m;      /* now cur_m! is stored */

        /* count digits 5 in the new factorial */
        size_t c5 = 0;
        for (size_t i = 0; i < fac_len; ++i)
            c5 += digit5[fac[i]];

        /* enlarge answer array if this count exceeds current capacity */
        if (c5 >= ans_cap) {
            size_t old = ans_cap;
            size_t new_cap = ans_cap ? ans_cap << 1 : 32;
            while (new_cap <= c5)
                new_cap <<= 1;
            ans = (seq_t*)realloc(ans, new_cap * sizeof(seq_t));
            memset(ans + old, 0, (new_cap - old) * sizeof(seq_t));
            ans_cap = new_cap;
        }

        /* record the first m that gives this number of 5‑digits */
        if (ans[c5] == 0)
            ans[c5] = (seq_t)cur_m;

        /* if we found the requested count, return */
        if (c5 == (size_t)n)
            return (seq_t)cur_m;
    }
}
