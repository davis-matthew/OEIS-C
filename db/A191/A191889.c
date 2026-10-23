/*
 * A191889  Ordered sequence of nonnegative differences 5*f-3*g, where f and g
 *          are positive Fibonacci numbers (A000045).
 * https://oeis.org/A191889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth nonnegative value of 5*f - 3*g where f and g are
 *            positive Fibonacci numbers
 * Range:     u8 n <= 52, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#include <algorithm>
#else
#include <stdlib.h>
#include <stdint.h>
#endif

/* ---------- C implementation ---------- */
#ifndef __cplusplus
/* maximum value storable in seq_t */
static const seq_t seq_t_max = (seq_t)-1;

/* comparator for qsort */
static int seq_cmp(const void *a, const void *b)
{
    const seq_t va = *(const seq_t *)a;
    const seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* static state */
static seq_t *sequence = NULL;
static size_t sequence_len = 0;
static int built = 0;

static void build_sequence(void)
{
    const idx_t K = 2000;                       /* number of Fibonacci terms to generate */
    const unsigned __int128 seq_t_max128 = (unsigned __int128)seq_t_max;

    /* generate Fibonacci numbers in 128‑bit */
    unsigned __int128 *fibs = (unsigned __int128*)malloc((K + 1) * sizeof(unsigned __int128));
    fibs[0] = 0;
    fibs[1] = 1;
    fibs[2] = 1;
    for (idx_t i = 3; i <= K; ++i) fibs[i] = fibs[i-1] + fibs[i-2];

    /* estimate storage for raw differences */
    size_t cap = (size_t)K * (size_t)K / 2 + 1;
    seq_t *tmp = (seq_t*)malloc(cap * sizeof(seq_t));
    size_t count = 0;

    for (idx_t i = 1; i <= K; ++i) {
        unsigned __int128 five_f = fibs[i] * 5;
        for (idx_t j = 1; j <= K; ++j) {
            unsigned __int128 three_g = fibs[j] * 3;
            if (five_f < three_g) break;
            unsigned __int128 diff = five_f - three_g;
            if (diff > seq_t_max128) continue;      /* skip values that overflow seq_t */
            if (count == cap) {
                cap *= 2;
                tmp = (seq_t*)realloc(tmp, cap * sizeof(seq_t));
            }
            tmp[count++] = (seq_t)diff;
        }
    }

    /* sort and deduplicate */
    qsort(tmp, count, sizeof(seq_t), seq_cmp);

    /* allocate final array */
    sequence = (seq_t*)malloc(count * sizeof(seq_t));
    sequence_len = 0;
    for (size_t i = 0; i < count; ++i) {
        if (sequence_len == 0 || sequence[sequence_len-1] != tmp[i]) {
            sequence[sequence_len++] = tmp[i];
        }
    }

    /* clean up */
    free(tmp);
    free(fibs);
    built = 1;
}
#endif /* __cplusplus */

/* ---------- C++ implementation ---------- */
#ifdef __cplusplus
/* static state */
static std::vector<seq_t> sequence;
static bool built = false;

static void build_sequence(void)
{
    const idx_t K = 2000;                  /* number of Fibonacci terms to generate */

    /* generate Fibonacci numbers */
    std::vector<seq_t> fibs(K + 1);
    fibs[0] = seq_t(0);
    fibs[1] = seq_t(1);
    fibs[2] = seq_t(1);
    for (idx_t i = 3; i <= K; ++i) {
        fibs[i] = fibs[i-1] + fibs[i-2];
    }

    /* collect raw differences */
    std::vector<seq_t> tmp;
    tmp.reserve(K * K / 2);

    for (idx_t i = 1; i <= K; ++i) {
        seq_t left = fibs[i] * seq_t(5);
        for (idx_t j = 1; j <= K; ++j) {
            seq_t right = fibs[j] * seq_t(3);
            if (left < right) break;
            tmp.push_back(left - right);
        }
    }

    /* sort and deduplicate */
    std::sort(tmp.begin(), tmp.end());
    tmp.erase(std::unique(tmp.begin(), tmp.end()), tmp.end());

    sequence.swap(tmp);
    built = true;
}
#endif /* __cplusplus */

/* ---------- public accessor ---------- */
seq_t a(idx_t n)
{
    if (!built) build_sequence();
# ifndef __cplusplus
    /* C: assume caller never requests out of bounds */
    return sequence[n - 1];
# else
    /* C++: guard against out of range */
    if (n <= 0 || (size_t)n > sequence.size()) return seq_t(0);
    return sequence[n - 1];
# endif
}
