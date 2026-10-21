/*
 * A080471  a(n) is the smallest Fibonacci number that is obtained by placing
 *          digits anywhere in n; a(n) = n if n is a Fibonacci number.
 * https://oeis.org/A080471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 111, u64 n <= 1083,
 *            u128 n <= 10000, big any n
 * Helpers:   is_subseq, maxsize, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A080471:  a(n) = the smallest Fibonacci number that can be
   obtained by inserting the digits of n anywhere in n (in any order).
   If n itself is a Fibonacci number, a(n)=n.
   The implementation keeps the list of Fibonacci numbers as both the
   decimal string (for subsequence testing) and the numeric value in the
   seq_t type (for returns and comparisons).                 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_subseq.h"
#include "../utils/maxsize.h"
#include "../utils/seq_from_str.h"

/* ------------------------------------------------------------------ */
/*  Global state                                                    */
/* ------------------------------------------------------------------ */
static size_t fib_count = 0;   /* number of Fibonacci numbers stored (1‑based) */
static size_t fib_cap   = 0;   /* capacity of the fib arrays                 */
static char **fib_str   = NULL;/* decimal string of Fibonacci number         */
static seq_t *fib_val   = NULL;/* numeric value of Fibonacci number          */

static seq_t *cache     = NULL;/* cache of computed a(n) values              */
static size_t cache_cap = 0;    /* capacity of cache                         */

/* ------------------------------------------------------------------ */
/*  Utility functions                                               */

/* Convert an integer into a decimal string (non‑empty, caller supplies buffer). */
static void idx_to_str(idx_t n, char *buf)
{
    if (n == 0) { buf[0] = '0'; buf[1]='\0'; return; }
    idx_t tmp = n;
    char rev[32];
    int pos = 0;
    while (tmp) { rev[pos++] = '0' + (tmp % 10); tmp /= 10; }
    for (int i=0;i<pos;i++) buf[i] = rev[pos-1-i];
    buf[pos] = '\0';
}

/* Convert a seq_t value to a decimal string.  The buffer is filled
   with the string and nul terminated.  The buffer must be large
   enough to hold the result (400 characters is safe for all
   tests).                                                     */
static void seq_to_str(seq_t v, char *buf)
{
    if (v == 0) { buf[0]='0'; buf[1]='\0'; return; }
    seq_t tmp = v;
    char rev[400];
    int pos = 0;
    while (tmp) {
        seq_t digit = tmp % 10;
        rev[pos++] = '0' + (int)digit;
        tmp /= 10;
    }
    for (int i=0;i<pos;i++) buf[i] = rev[pos-1-i];
    buf[pos] = '\0';
}

/* ------------------------------------------------------------------ */
/*  Fibonacci generation helpers                                     */
/* ------------------------------------------------------------------ */
static void ensure_fib(idx_t req)
{
    if (req <= fib_count) return;
    if (fib_cap == 0) {
        fib_cap = 8;
        fib_str = (char**)malloc((fib_cap+1)*sizeof(char*));
        fib_val = (seq_t*)malloc((fib_cap+1)*sizeof(seq_t));
    }
    if (fib_count < 2) {
        fib_str[1] = strdup("1"); fib_val[1] = 1;
        fib_str[2] = strdup("1"); fib_val[2] = 1;
        fib_count = 2;
    }
    while (fib_count < (size_t)req) {
        if (fib_count+2 > fib_cap) {
            fib_cap <<= 1;
            fib_str = (char**)realloc(fib_str,(fib_cap+1)*sizeof(char*));
            fib_val = (seq_t*)realloc(fib_val,(fib_cap+1)*sizeof(seq_t));
        }
        seq_t next_val = fib_val[fib_count] + fib_val[fib_count-1];
        char tmpbuf[400];
        seq_to_str(next_val,tmpbuf);
        fib_str[++fib_count] = strdup(tmpbuf);
        fib_val[fib_count] = next_val;
    }
}

/* ------------------------------------------------------------------ */
/*  Cache helpers                                                    */
/* ------------------------------------------------------------------ */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    size_t new_cap = cache_cap?cache_cap:8;
    while (new_cap <= n) new_cap <<= 1;
    cache = (seq_t*)realloc(cache,new_cap*sizeof(seq_t));
    for (size_t i=cache_cap;i<new_cap;i++) cache[i]=0;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Public function a(idx_t n)                                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, return 0 */

    ensure_cache(n);
    if (cache[n] != 0) return cache[n];

    /* convert n to string and numeric value */
    char n_str[32];
    idx_to_str(n,n_str);
    seq_t n_val = seq_from_str(n_str);

    /* make sure we have enough Fibonacci numbers */
    ensure_fib(1);

    /* quick test: does n itself appear in the Fibonacci list? */
    /* binary style linear search as the list is short for our limits */
    for (idx_t i=1;i<=fib_count;i++) {
        if (fib_val[i] == n_val) {
            cache[n] = n_val;
            return n_val;
        }
    }

    /* look for the first Fibonacci number that contains n_str */
    for (idx_t i=1;i<=fib_count;i++) {
        if (is_subseq(fib_str[i],n_str)) {
            cache[n] = fib_val[i];
            return fib_val[i];
        }
    }

    /* generate further Fibonacci numbers until a match is found */
    idx_t i = fib_count+1;
    for (;;) {
        ensure_fib(i);
        if (is_subseq(fib_str[i],n_str)) {
            cache[n] = fib_val[i];
            return fib_val[i];
        }
        ++i;
    }
}
