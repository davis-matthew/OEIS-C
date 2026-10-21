/*
 * A083126  Palindromes arising in A083125. a(n) = A083125(n)*A083125(n+1).
 * https://oeis.org/A083126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A083125(n) * A083125(n+1)
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_palindrome, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"
#include "../utils/seq_to_str.h"

#ifdef __cplusplus
#include <unordered_set>
#include <string>
#endif

/* Cache for the A083125 sequence (1‑based). */
static seq_t *A = NULL;          /* A[1..Alen] */
static size_t Acap = 0;
static idx_t Alen = 0;           /* number of elements stored */

/* Storage of used numbers.
   In C we use a boolean bitmap, in C++ an unordered_set of string keys. */
#ifndef __cplusplus
static bool *used = NULL;        /* bitmap of used numbers indexed by integer value */
static size_t used_cap = 0;
#else
static std::unordered_set<std::string> used_set;
#endif

/* Ensure bitmap can hold index idx (C only). */
#ifndef __cplusplus
static void ensure_used_capacity(size_t idx)
{
    if (idx + 1 < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap : 128;
    while (new_cap <= idx)
        new_cap <<= 1;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp)
        abort();
    for (size_t i = used_cap; i < new_cap; ++i)
        tmp[i] = false;
    used = tmp;
    used_cap = new_cap;
}
#endif

/* Ensure array A can store at least 'idx' elements (1‑based). */
static void ensure_A_capacity(idx_t idx)
{
    if ((size_t)idx <= Acap)
        return;
    size_t new_cap = Acap ? Acap : 16;
    while ((size_t)new_cap < (size_t)idx)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(A, (new_cap + 1) * sizeof(seq_t));
    if (!tmp)
        abort();
    A = tmp;
    Acap = new_cap;
}

/* Generate the next element of A083125 and append to A. */
static void generate_next_A(void)
{
    seq_t prev = A[Alen];          /* previous term */
    seq_t candidate = 1;

    for (;; ++candidate) {
#ifndef __cplusplus
        /* C implementation uses bitmap index. */
        size_t ci = (size_t)candidate;
        ensure_used_capacity(ci);
        if (used[ci])
            continue;
#else
        /* C++ implementation uses string set. */
        char buf[512];
        size_t len;
        seq_to_str(candidate, buf, &len);
        std::string key(buf, len);
        if (used_set.find(key) != used_set.end())
            continue;
#endif
        seq_t prod = prev * candidate;
        if (is_palindrome(prod, 10))
            break;
    }

    /* Store new term. */
    ensure_A_capacity(Alen + 1);
    ++Alen;
    A[Alen] = candidate;

    /* Mark as used. */
#ifndef __cplusplus
    used[(size_t)candidate] = true;
#else
    char buf[512];
    size_t len;
    seq_to_str(candidate, buf, &len);
    used_set.emplace(buf, buf + len);
#endif
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* undefined for n<1, guard anyway */

    /* Initialise on first call. */
    if (!A) {
        Acap = 32;
        A = (seq_t *)calloc(Acap + 1, sizeof(seq_t));
        if (!A)
            abort();
        A[1] = 1;
        Alen = 1;

#ifndef __cplusplus
        used_cap = 256;
        used = (bool *)calloc(used_cap, sizeof(bool));
        if (!used)
            abort();
        used[1] = true;
#else
        used_set.clear();
        used_set.reserve(256);
        used_set.emplace("1", "1" + 1); /* string "1" */
#endif
    }

    /* Ensure we have computed up to A[n+1]. */
    idx_t need = n + 1;
    while (Alen < need)
        generate_next_A();

    return A[n] * A[n + 1];
}
