/*
 * A063579  Smallest k such that 4^k has exactly n 4's in its decimal
 *          representation.
 * https://oeis.org/A063579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k such that 4^k has exactly n digits '4' in its
 *            decimal representation, with a(0) being the first such k
 * Range:     u8 n <= 21, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Dynamic storage for the sequence values.
   cache[n] holds a(n); 0 means “not yet computed”. */
static seq_t *cache = NULL;      /* array of a(n) values */
static size_t cache_len  = 0;    /* allocated number of entries */

/* Big‑integer representation of 4^k in base 10.
   digits[0] is the least significant digit. */
static uint8_t *digits = NULL;   /* decimal digits */
static size_t digit_cap = 0;     /* allocated capacity */
static size_t digit_len = 0;     /* current number of digits */

/* Current exponent k (ie. after the last multiplication). */
static idx_t current_k = 0;

/* Flag signalling whether the static data has been initialised. */
static int initialized = 0;

/* Ensure that the digits buffer has at least `need` bytes of space. */
static void ensure_digits_cap(size_t need)
{
    if (digit_cap >= need) return;
    size_t newcap = digit_cap ? digit_cap * 2 : 64;
    while (newcap < need) newcap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(digits, newcap);
    if (!tmp) abort();
    digits = tmp;
    digit_cap = newcap;
}

/* Ensure that the cache array has at least `need` entries. */
static void ensure_cache_cap(size_t need)
{
    if (cache_len >= need) return;
    size_t newcap = cache_len ? cache_len * 2 : 8;
    while (newcap < need) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) abort();
    if (newcap > cache_len)            /* zero‑initialise the new part */
        memset(tmp + cache_len, 0, (newcap - cache_len) * sizeof(seq_t));
    cache = tmp;
    cache_len = newcap;
}

/* Must be called before any other operation. */
static void init(void)
{
    if (initialized) return;
    initialized = 1;

    /* Start with 4^1 = 4. */
    ensure_digits_cap(1);
    digit_len = 1;
    digits[0] = 4;
    current_k = 1;

    /* Count the single '4'. */
    ensure_cache_cap(2);           /* indices 0 and 1 possible */
    cache[1] = (seq_t)1;           /* a(1) = 1 */
}

/* Multiply the stored number by 4, update the digit array,
   and return the number of '4' digits in the new representation. */
static idx_t next_power_and_count(void)
{
    uint8_t carry = 0;
    idx_t cnt = 0;                 /* count of '4' digits in the new number */

    for (size_t i = 0; i < digit_len; ++i) {
        uint8_t prod = digits[i] * 4 + carry;
        uint8_t newd = prod % 10;
        digits[i] = newd;
        if (newd == 4) ++cnt;
        carry = prod / 10;
    }

    /* Handle remaining carry as new most significant digits. */
    while (carry) {
        if (digit_len == digit_cap) ensure_digits_cap(digit_cap << 1);
        uint8_t newd = carry % 10;
        digits[digit_len++] = newd;
        if (newd == 4) ++cnt;
        carry /= 10;
    }

    return cnt;
}

/* Compute the sequence up to at least index n. */
static void compute_up_to(idx_t n)
{
    init(); /* Ensure data structures exist. */

    /* Mark cache[n] as 'ready' once it receives a non‑zero value. */
    while (cache[n] == 0) {
        idx_t cnt = next_power_and_count();  /* new count for 4^(current_k + 1) */
        ++current_k;                         /* now we have 4^current_k */

        if ((size_t)cnt + 1 > cache_len) ensure_cache_cap(cnt + 2);
        if (cache[cnt] == 0) {   /* first occurrence of this count */
            cache[cnt] = (seq_t)current_k;
        }
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
