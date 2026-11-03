/*
 * A116020  Numbers n such that sigma(n) - phi(n) is a repdigit greater than
 *          2.
 * https://oeis.org/A116020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   get_digits, mem, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for n such that sigma(n) - phi(n) is a repdigit greater than 2 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_digits.h"
#include "../utils/totient_seq.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined for n < 1 */

    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t sz    = 0;
    static uint64_t next = 1;                   /* next candidate to test */

    while (sz < n) {
        idx_t cand = next++;
        seq_t sig  = sigma_of(cand);
        seq_t phi  = totient_seq((seq_t)cand);
        seq_t diff = sig - phi;
        /* diff must be > 2 and a repdigit */
        if (diff > (seq_t)2) {
            size_t len;
            uint8_t buf[128];
            get_digits(diff, buf, &len);
            if (len > 0) {
                bool rep = true;
                uint8_t d = buf[0];
                for (size_t i = 1; i < len; ++i) {
                    if (buf[i] != d) { rep = false; break; }
                }
                if (rep) {
                    ensure_capacity(sz + 1, &cap, OEISC_VPP(&cache), sizeof(seq_t));
                    cache[sz++] = (seq_t)cand;
                }
            }
        }
    }
    return cache[n - 1];
}
