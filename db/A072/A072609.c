/*
 * A072609  Changing of parity of remainder A072608(n) from alternation
 *          [..010101..] to steadily 1-range [...1111..]. AC-range corresponds
 *          to 0, while DC-range labeled by 1.
 * https://oeis.org/A072609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = (prime(n) mod n mod 2) * (prime(n+1) mod (n+1) mod 2) =
 *            A072608(n)*A072608(n+1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *A072609_pr = NULL;
static size_t A072609_cnt = 0;
static uint64_t A072609_lim = 0;

static void A072609_ensure(uint64_t need)
{
    while (A072609_cnt < need) {
        uint64_t lim = A072609_lim ? A072609_lim * 2 : 4096;
        unsigned char *s = (unsigned char *)malloc((size_t)lim + 1);
        if (!s) abort();
        memset(s, 1, (size_t)lim + 1);
        s[0] = 0; s[1] = 0;
        for (uint64_t i = 2; i * i <= lim; i++)
            if (s[i])
                for (uint64_t j = i * i; j <= lim; j += i) s[j] = 0;
        size_t c = 0;
        for (uint64_t i = 2; i <= lim; i++) if (s[i]) c++;
        uint64_t *p = (uint64_t *)realloc(A072609_pr, c * sizeof(uint64_t));
        if (!p) abort();
        A072609_pr = p;
        size_t k = 0;
        for (uint64_t i = 2; i <= lim; i++) if (s[i]) p[k++] = i;
        A072609_cnt = c;
        A072609_lim = lim;
        free(s);
    }
}

static unsigned A072609_par(uint64_t n)
{
    return (unsigned)((A072609_pr[n - 1] % n) & 1);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    A072609_ensure((uint64_t)n + 1);
    return (seq_t)(A072609_par((uint64_t)n) * A072609_par((uint64_t)n + 1));
}
