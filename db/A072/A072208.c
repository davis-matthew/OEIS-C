/*
 * A072208  a(n)-th factorial is the smallest factorial containing exactly n
 *          8's, or 0 if no such number exists.
 * https://oeis.org/A072208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest m such that m! contains exactly n digits ‘8’
 *            (0 if no such m)
 * Range:     u8 n <= 45, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------ */
/* Basic big integer using base 10^4 (base=10000)                */
/* ------------------------------------------------------------ */
#define BASE 10000U
#define BASE_DIGITS 4U

static uint32_t *fact_digits = NULL;    /* little‑endian blocks */
static size_t fact_len   = 0;           /* number of blocks used */
static size_t fact_cap   = 0;           /* allocated capacity */
static idx_t   fact_max_m = 0;          /* last computed factorial index (fact_max_m!) */

/* ------------------------------------------------------------ */
/* Mapping from 8‑count to minimal m (ans[n] = m or -1)        */
/* ------------------------------------------------------------ */
static int  *ans_array   = NULL;            /* ans[8-count] = minimal m */
static size_t ans_cap    = 0;                 /* capacity of ans_array */

/* ------------------------------------------------------------ */
/* Table with number of '8's in a 4‑digit block (with leading zeros)   */
/* ------------------------------------------------------------ */
static uint8_t count8_table[BASE] = {0};

/* ------------------------------------------------------------ */
static void init_tables(void)
{
    if (count8_table[0] != 0)
        return;  /* already initialised */
    for (uint32_t i = 0; i < BASE; ++i) {
        uint32_t v = i;
        uint8_t c = 0;
        for (uint8_t j = 0; j < BASE_DIGITS; ++j) {
            if (v % 10 == 8) c++;
            v /= 10;
        }
        count8_table[i] = c;
    }
}

/* ------------------------------------------------------------ */
static void init_fact(void)
{
    fact_cap   = 64;
    fact_digits = (uint32_t *)malloc(fact_cap * sizeof(uint32_t));
    fact_digits[0] = 1;  /* 1! = 1 */
    fact_len   = 1;
    fact_max_m = 1;
}

/* ------------------------------------------------------------ */
static void init_ans_array(void)
{
    ans_cap = 20001;          /* sufficient for typical use (n up to ~20000) */
    ans_array = (int *)malloc(ans_cap * sizeof(int));
    /* initialise with -1 */
    for (size_t i = 0; i < ans_cap; ++i) ans_array[i] = -1;
}

/* ------------------------------------------------------------ */
static void grow_ans_array(size_t needed)
{
    while (ans_cap <= needed) {
        ans_cap <<= 1;
    }
    ans_array = (int *)realloc(ans_array, ans_cap * sizeof(int));
    /* initialise new part */
    for (size_t i = 0; i < ans_cap; ++i)
        if (ans_array[i] == 0 && i > 0) /* leave old entries alone */
            ans_array[i] = -1;
}

/* ------------------------------------------------------------ */
static void big_mul(uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < fact_len; ++i) {
        uint64_t prod = (uint64_t)fact_digits[i] * m + carry;
        fact_digits[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    while (carry) {
        if (fact_len == fact_cap) {
            fact_cap <<= 1;
            fact_digits = (uint32_t *)realloc(fact_digits, fact_cap * sizeof(uint32_t));
        }
        fact_digits[fact_len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* ------------------------------------------------------------ */
static int count8_digits(void)
{
    int cnt = 0;
    for (size_t i = 0; i + 1 < fact_len; ++i)
        cnt += count8_table[fact_digits[i]];
    /* most significant block without leading zeros */
    uint32_t msb = fact_digits[fact_len - 1];
    while (msb) {
        if (msb % 10 == 8) cnt++;
        msb /= 10;
    }
    return cnt;
}

/* ------------------------------------------------------------ */
static idx_t MAX_FACTORIAL_LIMIT = 30000U;  /* higher may be needed but keeps time reasonable */

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        init_tables();
        init_fact();
        init_ans_array();
        initialized = 1;
    }

    /* assure ans array big enough for current query */
    if ((size_t)n >= ans_cap)
        grow_ans_array((size_t)n);

    /* if answer already known, return it */
    if (ans_array[n] != -1)
        return (seq_t)ans_array[n];

    /* compute factorials until we find n or exceed limit */
    while (fact_max_m < MAX_FACTORIAL_LIMIT) {
        idx_t next = fact_max_m + 1U;
        big_mul((uint32_t)next);
        fact_max_m = next;

        int cnt8 = count8_digits();

        /* store first occurrence for any cnt8 that hasn't been recorded yet */
        if ((size_t)cnt8 < ans_cap && ans_array[cnt8] == -1)
            ans_array[cnt8] = (int)fact_max_m;

        if (cnt8 == n) {
            ans_array[n] = (int)fact_max_m;   /* store it for future calls */
            return (seq_t)fact_max_m;
        }
    }
    /* if not found within limit, return 0 as per definition */
    return (seq_t)0;
}
