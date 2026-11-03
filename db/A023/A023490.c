/*
 * A023490  n-th non-Lucas number plus Fibonacci(n + 1).
 * https://oeis.org/A023490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000045(n + 2) + A090946(n + 2)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 184,
 *            big any n
 * Validated: standard, pass, 55 terms through n = 972 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

/* --- Lucas numbers (1‑based: L1 = 1, L2 = 3, Lk = Lk-1 + Lk-2) --- */
static seq_t *lucas = NULL;
static size_t lucas_len = 0, lucas_cap = 0;

static void extend_lucas_up_to(seq_t v)
{
    if (lucas == NULL) {
        lucas_cap = 4;
        lucas = mem_new(seq_t, lucas_cap);
        lucas[0] = (seq_t)1;   /* L1 */
        lucas[1] = (seq_t)3;   /* L2 */
        lucas_len = 2;
    }
    while (lucas_len > 0 && lucas[lucas_len-1] < v) {
        seq_t next = lucas[lucas_len-1] + lucas[lucas_len-2];
        if (lucas_len == lucas_cap) {
            lucas_cap *= 2;
            lucas = mem_resize(seq_t, lucas, lucas_cap);
        }
        lucas[lucas_len++] = next;
    }
}

static bool is_lucas(seq_t v)
{
    if (v == (seq_t)1 || v == (seq_t)3) return true;
    extend_lucas_up_to(v);
    for (size_t i = 0; i < lucas_len; ++i) {
        if (lucas[i] == v) return true;
        if (lucas[i] > v) break;
    }
    return false;
}

/* --- Non‑Lucas numbers (starting with 0,2,5,6,8,…) --- */
static seq_t *nonlucas = NULL;
static size_t nonlucas_len = 0, nonlucas_cap = 0;
static seq_t candidate = (seq_t)0;   /* next integer to test */

static void ensure_nonlucas_len(size_t target_len)
{
    if (nonlucas_len >= target_len) return;

    if (nonlucas_cap < target_len) {
        nonlucas_cap = target_len;
        nonlucas = mem_resize(seq_t, nonlucas, nonlucas_cap);
    }

    while (nonlucas_len < target_len) {
        if (!is_lucas(candidate)) {
            nonlucas[nonlucas_len++] = candidate;
        }
        ++candidate;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    /* Fibonacci part */
    seq_t f = fib(n + 2);

    /* Non–Lucas part: index n+2 (1‑indexed) */
    size_t idx = (size_t)(n + 2);
    ensure_nonlucas_len(idx);
    seq_t nl = nonlucas[idx - 1];   /* zero‑based array access */

    return f + nl;
}
