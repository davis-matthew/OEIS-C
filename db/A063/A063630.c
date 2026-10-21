/*
 * A063630  Smallest k such that 9^k has exactly n 4's in its decimal
 *          representation.
 * https://oeis.org/A063630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = smallest k such that 9^k has exactly n digits equal to 4
   (OEIS A063630).  */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/*  seq_t and idx_t are provided by the build environment.      */
/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* --- static data -------------------------------------------------- */
    static bool   *seen      = NULL;   /* seen[count]  */
    static seq_t  *res       = NULL;   /* minimal k for count */
    static uint8_t *digits   = NULL;   /* decimal digits LSB→MSB */
    static size_t  cap_seen  = 0;      /* capacity of seen/res   */
    static size_t  cap_digits = 0;     /* capacity of digits     */
    static size_t  len_digits  = 0;     /* current length of digits */
    static idx_t   cur_k       = 1;     /* current exponent of 9   */
    static idx_t   filled_upto = 0;     /* next count index still missing */
    static bool    initialized = false;

    /* --- lazy initialization ------------------------------------------- */
    if (!initialized)
    {
        /* allocate space for the first power 9^1 = 9 */
        if (cap_digits < 1) {
            cap_digits = 8;
            digits = (uint8_t *)malloc(cap_digits * sizeof(uint8_t));
            if (!digits) abort();
        }
        digits[0] = 9;      /* 9^1 = 9 */
        len_digits = 1;

        /* allocate space for seen/res arrays */
        if (cap_seen < 1) {
            cap_seen = 8;
            seen = (bool *)malloc(cap_seen * sizeof(bool));
            res  = (seq_t *)malloc(cap_seen * sizeof(seq_t));
            if (!seen || !res) abort();
        }
        /* count of 4’s in 9^1 is 0, so a(0)=1 */
        seen[0] = true;
        res[0]  = (seq_t)1;  /* a(0) = 1 */

        /* we have information for n = 0 only yet */
        cur_k       = 1;
        filled_upto = 1;
        initialized = true;
    }

    /* --- extend seen/res arrays if n exceeds capacity ------------------ */
    if ((size_t)n >= cap_seen) {
        size_t new_cap = cap_seen ? cap_seen : 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        bool   *new_seen = (bool *)realloc(seen, new_cap * sizeof(bool));
        seq_t  *new_res  = (seq_t *)realloc(res,  new_cap * sizeof(seq_t));
        if (!new_seen || !new_res) abort();
        memset(new_seen + cap_seen, 0, (new_cap - cap_seen) * sizeof(bool));
        seen = new_seen;
        res  = new_res;
        cap_seen = new_cap;
    }

    /* --- generate 9^k until we have determined a(n) ------------------- */
    while (filled_upto <= n) {
        /* advance to next exponent */
        ++cur_k;

        /* multiply the decimal representation by 9 */
        uint32_t carry = 0;
        for (size_t i = 0; i < len_digits; ++i) {
            uint32_t prod = digits[i] * 9u + carry;
            digits[i] = (uint8_t)(prod % 10u);
            carry     = prod / 10u;
        }
        while (carry) {
            if (len_digits >= cap_digits) {
                size_t new_cap = cap_digits ? cap_digits : 8;
                while (new_cap <= len_digits) new_cap <<= 1;
                uint8_t *tmp = (uint8_t *)realloc(digits, new_cap * sizeof(uint8_t));
                if (!tmp) abort();
                digits = tmp;
                cap_digits = new_cap;
            }
            digits[len_digits++] = (uint8_t)(carry % 10u);
            carry /= 10u;
        }

        /* count digits equal to 4 */
        size_t cnt4 = 0;
        for (size_t i = 0; i < len_digits; ++i)
            if (digits[i] == 4u) ++cnt4;

        /* extend seen/res arrays if needed for this count */
        if (cnt4 >= cap_seen) {
            size_t new_cap = cap_seen ? cap_seen : 8;
            while (new_cap <= cnt4) new_cap <<= 1;
            bool   *new_seen = (bool *)realloc(seen, new_cap * sizeof(bool));
            seq_t  *new_res  = (seq_t *)realloc(res,  new_cap * sizeof(seq_t));
            if (!new_seen || !new_res) abort();
            memset(new_seen + cap_seen, 0, (new_cap - cap_seen) * sizeof(bool));
            seen = new_seen;
            res  = new_res;
            cap_seen = new_cap;
        }

        /* store minimal k for this count if first occurrence */
        if (!seen[cnt4]) {
            seen[cnt4] = true;
            res[cnt4]  = (seq_t)cur_k;
        }

        /* advance filled_upto while those counts are already known */
        while (filled_upto < (idx_t)cap_seen && seen[filled_upto])
            ++filled_upto;
    }

    return res[(size_t)n];
}
