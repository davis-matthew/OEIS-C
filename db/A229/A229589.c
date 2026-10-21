/*
 * A229589  Number of defective 3-colorings of a 4 X n 0..2 array connected
 *          horizontally and antidiagonally with exactly one mistake, and
 *          colors introduced in row-major 0..2 order.
 * https://oeis.org/A229589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A229589: Number of defective 3‑colorings of a 4×n array connected
   horizontally and antidiagonally with exactly one mistake.
   The sequence satisfies the recurrence
     a(n) = 18 a(n‑1) – 111 a(n‑2) + 282 a(n‑3) – 333 a(n‑4)
           + 180 a(n‑5) – 36 a(n‑6)        for n > 12
   with initial values
     a(1)=0, a(2)=224, a(3)=2160, a(4)=19764, a(5)=167364,
     a(6)=1375152, a(7)=11035044, a(8)=87040260, a(9)=677327004,
     a(10)=5213798784, a(11)=39777273072, a(12)=301217858676.
   This implementation computes terms on demand using the recurrence
   and memoises all previously computed values.  For integral types
   the intermediate arithmetic is performed with signed 128‑bit integers
   to avoid overflow; for non‑integral (arbitrary‑precision) types the
   operations are performed directly on the seq_t type. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static const seq_t init_vals[12] = {
    (seq_t)0ULL,
    (seq_t)224ULL,
    (seq_t)2160ULL,
    (seq_t)19764ULL,
    (seq_t)167364ULL,
    (seq_t)1375152ULL,
    (seq_t)11035044ULL,
    (seq_t)87040260ULL,
    (seq_t)677327004ULL,
    (seq_t)5213798784ULL,
    (seq_t)39777273072ULL,
    (seq_t)301217858676ULL
};

#ifdef __cplusplus
#include <vector>
#include <type_traits>
static std::vector<seq_t> cache;

/* recurrence for integral seq_t types using signed 128‑bit intermediate */
static inline seq_t recurrence_next_integral(const seq_t* arr, size_t i)
{
    __int128 next = 0;
    next += (__int128)arr[i-1] * 18;
    next += (__int128)arr[i-3] * 282;
    next += (__int128)arr[i-5] * 180;
    next -= (__int128)arr[i-2] * 111;
    next -= (__int128)arr[i-4] * 333;
    next -= (__int128)arr[i-6] * 36;
    return (seq_t)next;
}

/* recurrence for non‑integral (arbitrary‑precision) seq_t types */
static inline seq_t recurrence_next_big(const seq_t* arr, size_t i)
{
    seq_t next = arr[i-1] * 18;
    next += arr[i-3] * 282;
    next += arr[i-5] * 180;
    next -= arr[i-2] * 111;
    next -= arr[i-4] * 333;
    next -= arr[i-6] * 36;
    return next;
}
#else
/* C implementation assumes seq_t is an integral unsigned integer type */
static seq_t* cache = NULL;
static size_t cache_size = 0;

static inline seq_t recurrence_next_integral(const seq_t* arr, size_t i)
{
    __int128 next = 0;
    next += (__int128)arr[i-1] * 18;
    next += (__int128)arr[i-3] * 282;
    next += (__int128)arr[i-5] * 180;
    next -= (__int128)arr[i-2] * 111;
    next -= (__int128)arr[i-4] * 333;
    next -= (__int128)arr[i-6] * 36;
    return (seq_t)next;
}
#endif

seq_t a(idx_t n)
{
    if (n == 0ULL) return (seq_t)0;          /* undefined offset */

    if (n <= 12ULL) return init_vals[n-1];

#ifdef __cplusplus
    if (cache.empty()) {
        cache.reserve(12);
        for (idx_t i = 0; i < 12; ++i) cache.push_back(init_vals[i]);
    }
    if (n > cache.size()) {
        cache.resize(n);
        for (size_t i = 12; i < (size_t)n; ++i) {
            if constexpr (std::is_integral<seq_t>::value) {
                cache[i] = recurrence_next_integral(&cache[0], i);
            } else {
                cache[i] = recurrence_next_big(&cache[0], i);
            }
        }
    }
    return cache[n-1];
#else
    if (!cache) {
        cache = (seq_t*)malloc(12 * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure */
        for (idx_t i = 0; i < 12; ++i) cache[i] = init_vals[i];
        cache_size = 12;
    }
    if ((size_t)n > cache_size) {
        size_t new_size = (size_t)n;
        seq_t* tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache = tmp;
        for (size_t i = cache_size; i < new_size; ++i) {
            cache[i] = recurrence_next_integral(cache, i);
        }
        cache_size = new_size;
    }
    return cache[n-1];
#endif
}
