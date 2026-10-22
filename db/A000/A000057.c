/*
 * A000057  Primes that divide at least one term in every Fibonacci sequence.
 * https://oeis.org/A000057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over primes p (sieve), keeping those whose Fibonacci rank
 *            of apparition equals p + 1: p = 2, 3, or p == +-2 (mod 5) and
 *            Fibonacci((p+1)/q) != 0 (mod p) for every prime q dividing p + 1
 * Range:     u8 n <= 13, u16 n <= 1284, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a57_fibmod, a57_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/a57_fibmod.h"
#include "../utils/a57_ok.h"

static uint64_t *a57_terms = NULL;
static size_t a57_cnt = 0, a57_cap = 0;
static uint64_t a57_lim = 0;

static void a57_push(uint64_t v)
{
    if (a57_cnt == a57_cap) {
        a57_cap = a57_cap ? a57_cap * 2 : 1024;
        a57_terms = (uint64_t *)realloc(a57_terms, a57_cap * sizeof(uint64_t));
    }
    a57_terms[a57_cnt++] = v;
}

static void a57_extend(void)
{
    uint64_t newlim = a57_lim ? a57_lim * 2 : 4096;
    unsigned char *comp = (unsigned char *)malloc((size_t)newlim + 1);
    uint64_t i, j;
    memset(comp, 0, (size_t)newlim + 1);
    for (i = 2; i * i <= newlim; i++)
        if (!comp[i])
            for (j = i * i; j <= newlim; j += i) comp[j] = 1;
    for (i = (a57_lim < 2 ? 2 : a57_lim + 1); i <= newlim; i++)
        if (!comp[i] && a57_ok(i)) a57_push(i);
    free(comp);
    a57_lim = newlim;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while (a57_cnt < n) a57_extend();
    return (seq_t)a57_terms[n - 1];
}
