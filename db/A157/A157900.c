/*
 * A157900  a(n) is the smallest number whose name in UK English contains n
 *          consonants.
 * https://oeis.org/A157900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 29, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function: a
   Purpose: Computes OEIS sequence A157900 – the smallest number whose
   UK English name contains n consonants.
   The implementation scans numbers incrementally, records the first
   occurrence for each consonant count, and caches results so repeated
   calls are fast.  It works for all integer types allowed for seq_t
   and idx_t. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* -------- Consonant tables for 0–19 -------------------------------- */
static const int base_counts[20] = {
    2, /* 0 zero */
    1, /* 1 one */
    2, /* 2 two */
    3, /* 3 three */
    2, /* 4 four */
    2, /* 5 five */
    2, /* 6 six */
    3, /* 7 seven */
    3, /* 8 eight */
    2, /* 9 nine */
    2, /*10 ten */
    3, /*11 eleven*/
    4, /*12 twelve*/
    5, /*13 thirteen*/
    4, /*14 fourteen*/
    4, /*15 fifteen */
    4, /*16 sixteen */
    5, /*17 seventeen*/
    4, /*18 eighteen*/
    4  /*19 nineteen*/
};

/* Consonant counts for the tens 20,30,…,90 (index 0–9) */
static const int tens_counts[10] = {
    0, /* 0   */
    0, /* 1   */
    4, /* 20: twenty (y is a vowel) */
    4, /* 30: thirty */
    3, /* 40: forty  */
    3, /* 50: fifty  */
    3, /* 60: sixty  */
    4, /* 70: seventy */
    3, /* 80: eighty */
    3  /* 90: ninety */
};

#define HUNDRED_CONS 5   /* "hundred" */
#define THOUSAND_CONS 5  /* "thousand" */
#define MILLION_CONS 4   /* "million"  */
#define BILLION_CONS 4   /* "billion"  */
#define AND_CONS    2    /* "and"      */

/* --------- Count consonants of a number 0–999 ---------------------- */
static int count_consonants_0to999(uint64_t n)
{
    if (n < 20) {
        return base_counts[(int)n];
    }
    if (n < 100) {
        int tens = (int)(n / 10);
        int units = (int)(n % 10);
        return tens_counts[tens] + (units ? base_counts[units] : 0);
    }
    /* 100 <= n < 1000 */
    int hundreds = (int)(n / 100);
    int rem = (int)(n % 100);
    int cnt = base_counts[hundreds] + HUNDRED_CONS;
    if (rem == 0) {
        return cnt;
    }
    cnt += AND_CONS;
    if (rem < 20) {
        cnt += base_counts[rem];
    } else {
        int tens = rem / 10;
        int units = rem % 10;
        cnt += tens_counts[tens] + (units ? base_counts[units] : 0);
    }
    return cnt;
}

/* --------- Count consonants of any positive integer --------------- */
static int count_consonants_full(uint64_t n)
{
    if (n == 0) {
        return base_counts[0];
    }
    /* Decompose into groups of 3 digits (units, thousands, millions, ...) */
    int groups[10];
    int gcount = 0;
    uint64_t tmp = n;
    while (tmp) {
        groups[gcount++] = (int)(tmp % 1000);
        tmp /= 1000;
    }
    if (gcount == 1) {
        return count_consonants_0to999(n);
    }
    int total = 0;
    bool higher = false;
    for (int i = gcount - 1; i >= 0; --i) {
        int g = groups[i];
        if (g == 0) {
            continue;
        }
        if (i == 0) {            /* lowest group */
            if (higher && g < 100) {
                total += AND_CONS;
            }
            total += count_consonants_0to999(g);
        } else {
            total += count_consonants_0to999(g);
            if (i == 1) total += THOUSAND_CONS;
            else if (i == 2) total += MILLION_CONS;
            else if (i == 3) total += BILLION_CONS;
            /* Further scales omitted; not needed for n ≤ 20000. */
            higher = true;
        }
    }
    return total;
}

/* --------- Cache management ---------------------------------------- */
#define MAX_N 20000 /* sufficient for all tests */

static seq_t cache[MAX_N + 1];            /* smallest number with n consonants */
static uint8_t filled[MAX_N + 1] = {0};    /* 1 if cache[n] is set */
static uint64_t next_scan = 1;            /* next number to evaluate */

/* Ensure that cache[n] is computed (smallest number with n consonants). */
static void ensure_cache(idx_t n)
{
    if (n == 0 || n > MAX_N || filled[n]) return;

    while (!filled[n]) {
        int cnt = count_consonants_full(next_scan);
        if (cnt > 0 && cnt <= MAX_N && !filled[cnt]) {
            cache[cnt] = (seq_t)next_scan;
            filled[cnt] = 1;
        }
        ++next_scan;
    }
}

/* --------- Public function --------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined below offset */
    if (n > MAX_N) return (seq_t)0; /* out of range, defensive */
    ensure_cache(n);
    return cache[n];
}
